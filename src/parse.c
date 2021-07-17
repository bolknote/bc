/*
 * *****************************************************************************
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018-2021 Gavin D. Howard and contributors.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * *****************************************************************************
 *
 * Code common to the parsers.
 *
 */

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <limits.h>

#include <parse.h>
#include <program.h>
#include <vm.h>

void bc_parse_updateFunc(BcParse *p, size_t fidx) {
	p->fidx = fidx;
	p->func = bc_vec_item(&p->prog->fns, fidx);
}

inline void bc_parse_pushName(const BcParse *p, char *name, bool var) {
	bc_parse_pushIndex(p, bc_program_search(p->prog, name, var));
}

/**
 * Updates the function, then pushes the instruction and the index. This is a
 * convenience function.
 * @param p     The parser.
 * @param inst  The instruction to push.
 * @param idx   The index to push.
 */
static void bc_parse_update(BcParse *p, uchar inst, size_t idx) {
	bc_parse_updateFunc(p, p->fidx);
	bc_parse_push(p, inst);
	bc_parse_pushIndex(p, idx);
}

void bc_parse_addString(BcParse *p) {

	BcVec *strs = BC_IS_BC ? &p->func->strs : p->prog->strs;
	size_t idx;

	BC_SIG_LOCK;

#if BC_ENABLED

	// bc has different slab vectors than dc. dc also directly adds the string
	// as a function.
	if (BC_IS_BC) {

		char **str = bc_vec_pushEmpty(strs);

		// Figure out which slab vector to use.
		BcVec *slabs = p->fidx == BC_PROG_MAIN || p->fidx == BC_PROG_READ ?
		               &vm.main_slabs : &vm.other_slabs;

		*str = bc_slabvec_strdup(slabs, p->l.str.v);
		idx = strs->len - 1;
	}
#if DC_ENABLED
	else
#endif // DC_ENABLED
#endif // BC_ENABLED
#if DC_ENABLED
	{
		idx = bc_program_insertFunc(p->prog, p->l.str.v) - BC_PROG_REQ_FUNCS;
	}
#endif // DC_ENABLED

	// Push the string info.
	bc_parse_update(p, BC_INST_STR, idx);

	BC_SIG_UNLOCK;
}

static void bc_parse_addNum(BcParse *p, const char *string) {

	BcVec *consts = &p->func->consts;
	size_t idx;
	BcConst *c;
	BcVec *slabs;

	// Special case 0.
	if (bc_parse_zero[0] == string[0] && bc_parse_zero[1] == string[1]) {
		bc_parse_push(p, BC_INST_ZERO);
		return;
	}

	// Special case 1.
	if (bc_parse_one[0] == string[0] && bc_parse_one[1] == string[1]) {
		bc_parse_push(p, BC_INST_ONE);
		return;
	}

	// Get the index.
	idx = consts->len;

	BC_SIG_LOCK;

#if BC_ENABLED
	// Get the right slab.
	slabs = p->fidx == BC_PROG_MAIN || p->fidx == BC_PROG_READ || BC_IS_DC ?
	        &vm.main_const_slab : &vm.other_slabs;
#else // BC_ENABLED
	slabs = &vm.main_const_slab;
#endif // BC_ENABLED

	// Push an empty constant.
	c = bc_vec_pushEmpty(consts);

	// Set the fields.
	c->val = bc_slabvec_strdup(slabs, string);
	c->base = BC_NUM_BIGDIG_MAX;

	// We need this to be able to tell that the number has not been allocated.
	bc_num_clear(&c->num);

	bc_parse_update(p, BC_INST_NUM, idx);

	BC_SIG_UNLOCK;
}

void bc_parse_number(BcParse *p) {

#if BC_ENABLE_EXTRA_MATH
	char *exp = strchr(p->l.str.v, 'e');
	size_t idx = SIZE_MAX;

	// Do we have a number in scientific notation? If so, add a nul byte where
	// the e is.
	if (exp != NULL) {
		idx = ((size_t) (exp - p->l.str.v));
		*exp = 0;
	}
#endif // BC_ENABLE_EXTRA_MATH

	bc_parse_addNum(p, p->l.str.v);

#if BC_ENABLE_EXTRA_MATH
	// If we have a number in scientific notation...
	if (exp != NULL) {

		bool neg;

		// Figure out if the exponent is negative.
		neg = (*((char*) bc_vec_item(&p->l.str, idx + 1)) == BC_LEX_NEG_CHAR);

		// Add the number and instruction.
		bc_parse_addNum(p, bc_vec_item(&p->l.str, idx + 1 + neg));
		bc_parse_push(p, BC_INST_LSHIFT + neg);
	}
#endif // BC_ENABLE_EXTRA_MATH
}

void bc_parse_text(BcParse *p, const char *text, bool is_stdin) {

	// Make sure the pointer isn't invalidated.
	p->func = bc_vec_item(&p->prog->fns, p->fidx);
	bc_lex_text(&p->l, text, is_stdin);
}

void bc_parse_reset(BcParse *p) {

	BC_SIG_ASSERT_LOCKED;

	// Reset the function if it isn't main and switch to main.
	if (p->fidx != BC_PROG_MAIN) {
		bc_func_reset(p->func);
		bc_parse_updateFunc(p, BC_PROG_MAIN);
	}

	// Reset the lexer.
	p->l.i = p->l.len;
	p->l.t = BC_LEX_EOF;

#if BC_ENABLED
	if (BC_IS_BC) {

		// Get rid of the bc parser state.
		p->auto_part = false;
		bc_vec_npop(&p->flags, p->flags.len - 1);
		bc_vec_popAll(&p->exits);
		bc_vec_popAll(&p->conds);
		bc_vec_popAll(&p->ops);
	}
#endif // BC_ENABLED

	// Reset the program. This might clear the error.
	bc_program_reset(p->prog);

	// Jump if there is an error.
	if (BC_ERR(vm.status)) BC_JMP;
}

void bc_parse_free(BcParse *p) {

	BC_SIG_ASSERT_LOCKED;

	assert(p != NULL);

#if BC_ENABLED
	if (BC_IS_BC) {
		bc_vec_free(&p->flags);
		bc_vec_free(&p->exits);
		bc_vec_free(&p->conds);
		bc_vec_free(&p->ops);
		bc_vec_free(&p->buf);
		bc_slabvec_free(&p->slab);
	}
#endif // BC_ENABLED

	bc_lex_free(&p->l);
}

void bc_parse_init(BcParse *p, BcProgram *prog, size_t func) {

#if BC_ENABLED
	uint16_t flag = 0;
#endif // BC_ENABLED

	BC_SIG_ASSERT_LOCKED;

	assert(p != NULL && prog != NULL);

#if BC_ENABLED
	if (BC_IS_BC) {

		// We always want at least one flag set on the flags stack.
		bc_vec_init(&p->flags, sizeof(uint16_t), BC_DTOR_NONE);
		bc_vec_push(&p->flags, &flag);

		bc_vec_init(&p->exits, sizeof(BcInstPtr), BC_DTOR_NONE);
		bc_vec_init(&p->conds, sizeof(size_t), BC_DTOR_NONE);
		bc_vec_init(&p->ops, sizeof(BcLexType), BC_DTOR_NONE);
		bc_vec_init(&p->buf, sizeof(char), BC_DTOR_NONE);

		bc_slabvec_init(&p->slab);
		p->auto_part = false;
	}
#endif // BC_ENABLED

	bc_lex_init(&p->l);

	// Set up the function.
	p->prog = prog;
	bc_parse_updateFunc(p, func);
}
