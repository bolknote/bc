$ $
$ SPDX-License-Identifier: BSD-2-Clause
$ $
$ Copyright (c) 2018-2025 Gavin D. Howard and contributors.
$ $
$ Redistribution and use in source and binary forms, with or without
$ modification, are permitted provided that the following conditions are met:
$ $
$ * Redistributions of source code must retain the above copyright notice, this
$   list of conditions and the following disclaimer.
$ $
$ * Redistributions in binary form must reproduce the above copyright notice,
$   this list of conditions and the following disclaimer in the documentation
$   and/or other materials provided with the distribution.
$ $
$ THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
$ AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
$ IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
$ ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
$ LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
$ CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
$ SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
$ INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
$ CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
$ ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
$ POSSIBILITY OF SUCH DAMAGE.
$ $

$quote "

$ Typy błędów.
$set 1

1 "Błąd matematyczny:"
2 "Błąd parse'a:"
3 "Błąd biegu:"
4 "Błąd śmiertelny:"
5 "Ostrzeżenie:"

$ Błędy matematyczne.
$set 2

1 "liczba ujemna"
2 "numer nieintegracyjny"
3 "przelewanie: liczba nie mieści się w numerze sprzętowym"
4 "dzielenie przez 0"

$ Błędy Parse'a.
$set 3

1 "koniec akt"
2 "nieważny znak '%c'"
3 "koniec sznurka nie mógł być znaleziony"
4 "koniec komentarza nie mógł być znaleziony"
5 "nieważny token"
6 "nieważne wyrażenie"
7 "puste wyrażenie"
8 "nieprawidłowe polecenie drukowania lub przesyłania strumienia"
9 "nieprawidłowa definicja funkcji"
10 "nieprawidłowe przyporządkowanie: lewa strona musi być elementem scale, ibase, obase, last, var lub element array"
11 "nie znaleziono zmiennej automatycznej"
12 "parametr funkcji lub auto \"%s%s\" już istnieje"
13 "końca bloku nie można było znaleźć"
14 "nie może zwrócić wartości z funkcji void: %s()"
15 "var nie może być odniesieniem: %s"
16 "POSIX nie zezwala na nazwy dłuższe niż 1 znak: %s"
17 "POSIX nie pozwala na komentarze skryptu '#'"
18 "POSIX nie pozwala na użycie następującego słowa kluczowego: %s"
19 "POSIX nie dopuszcza kropki ('.') jako skrótu do ostatniego wyniku"
20 "POSIX wymaga nawiasów wokół wyrażeń zwrotnych"
21 "POSIX nie pozwala następującemu operatorowi: %s"
22 "POSIX nie pozwala na porównywanie operatorów na zewnątrz, jeśli deklaracje lub pętle"
23 "POSIX wymaga 0 lub 1 operatora porównawczego na jeden warunek"
24 "POSIX wymaga, aby wszystkie 3 części pętli nie były puste"
25 "POSIX wymaga nowej linii pomiędzy średnikiem a definicją funkcji"
26 "POSIX nie pozwala na notację wykładniczą"
27 "POSIX nie zezwala na odniesienia do tablicy jako parametrów funkcji"
28 "POSIX nie dopuszcza funkcji void"
29 "POSIX wymaga, aby lewe usztywnienie znajdowało się na tej samej linii co nagłówek funkcji"
30 "POSIX nie pozwala na przypisywanie ciągów znaków do zmiennych lub tablic"

$ Błędy Runtime'u.
$set 4

1 "nieprawidłowa ibase: musi być [%lu, %lu]"
2 "nieprawidłowa obase: musi być [%lu, %lu]"
3 "nieprawidłowa scale: musi być [%lu, %lu]"
4 "nieprawidłowe wyrażenie read()"
5 "powtarzalne wywołanie read()"
6 "element zmienny lub tablicowy jest niewłaściwym typem"
7 "stos ma zbyt mało elementów"
8 "stos dla rejestru \"%s\" ma zbyt mało elementów"
9 "niewłaściwa liczba parametrów; potrzeba %zu, mają %zu"
10 "niezdefiniowana funkcja: %s()"
11 "nie może użyć wartości pustej w wyrażeniu"

$ Fatalne błędy.
$set 5

1 "Alokacja pamięci nie powiodła się"
2 "Błąd we/wy"
3 "nie mógł otworzyć pliku: %s"
4 "plik nie jest tekstem: %s"
5 "ścieżka to katalog: %s"
6 "nieprawidłowa opcja wiersza poleceń: \"%s\""
7 "opcja wymaga argumentu: '%c' (\"%s\")"
8 "opcja nie wymaga żadnych argumentów: '%c' (\"%s\")"
9 "nieprawidłowa argument opcja wiersza poleceń: \"%s\""
