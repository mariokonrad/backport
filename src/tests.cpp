#include "string_view.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

using namespace backport::cpp11;

TEST_SUITE("construction");
TEST_CASE("default")
{
	CHECK_NOTHROW(string_view v);
}

TEST_CASE("copy constructor")
{
	string_view a;
	CHECK_NOTHROW(string_view v{a});
}

TEST_CASE("from C string and count")
{
	static const char * s = "Hello World";
	CHECK_NOTHROW(string_view v(s, 5));
}

TEST_CASE("from C string")
{
	static const char * s = "Hello World";
	CHECK_NOTHROW(string_view v(s));
}
TEST_SUITE_END();

TEST_SUITE("assignment");
TEST_CASE("copy assignment")
{
	string_view a{"Hello World"};
	string_view b{"Hallo Welt"};

	string_view * p = &a;
	p->remove_suffix(6);

	b = a;
	CHECK(b.size() == 5u);
	CHECK(std::strncmp(b.data(), "Hello", b.size()) == 0);
}
TEST_SUITE_END();

TEST_SUITE("iterators");
TEST_CASE("default construction")
{
	CHECK_NOTHROW(string_view::const_iterator i{});
}

TEST_CASE("copy assignment from string_view")
{
	string_view v{"Hello World"};
	const string_view::const_iterator j = v.begin();
	string_view::const_iterator i;
	CHECK_NOTHROW(i = j);
}

TEST_CASE("move assignment from string_view")
{
	string_view v{"Hello World"};
	string_view::const_iterator i;
	CHECK_NOTHROW(i = v.begin());
}

TEST_CASE("comparison equal")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	string_view::const_iterator j = v.begin();
	CHECK(i == j);
}

TEST_CASE("comparison not equal")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	string_view::const_iterator j = v.begin();
	CHECK(!(i != j));
}

TEST_CASE("character dereference")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	CHECK(*i == 'H');
}

TEST_CASE("character indexed dereference")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	CHECK(i[0] == 'H');
	CHECK(i[1] == 'e');
	CHECK(i[2] == 'l');
	CHECK(i[3] == 'l');
	CHECK(i[4] == 'o');
}

TEST_CASE("access")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	CHECK(i->size() == 11u);
}

TEST_CASE("prefix increment")
{
	string_view v{"Hello"};
	string_view::const_iterator i = v.begin();
	CHECK(*i == 'H');
	++i;
	CHECK(*i == 'e');
	++i;
	CHECK(*i == 'l');
	++i;
	CHECK(*i == 'l');
	++i;
	CHECK(*i == 'o');
	++i;
	CHECK(*i == '\0');
	++i;
	CHECK(*i == '\0');
}

TEST_CASE("postfix increment")
{
	string_view v{"Hello"};
	string_view::const_iterator i = v.begin();
	CHECK(*i == 'H');
	i++;
	CHECK(*i == 'e');
	i++;
	CHECK(*i == 'l');
	i++;
	CHECK(*i == 'l');
	i++;
	CHECK(*i == 'o');
	i++;
	CHECK(*i == '\0');
	i++;
	CHECK(*i == '\0');
}

TEST_CASE("prefix decrement")
{
	string_view v{"Hello"};
	string_view::const_iterator i = v.end();
	CHECK(*i == '\0');
	--i;
	CHECK(*i == 'o');
	--i;
	CHECK(*i == 'l');
	--i;
	CHECK(*i == 'l');
	--i;
	CHECK(*i == 'e');
	--i;
	CHECK(*i == 'H');
	--i;
	CHECK(*i == 'H');
}

TEST_CASE("postfix decrement")
{
	string_view v{"Hello"};
	string_view::const_iterator i = v.end();
	CHECK(*i == '\0');
	i--;
	CHECK(*i == 'o');
	i--;
	CHECK(*i == 'l');
	i--;
	CHECK(*i == 'l');
	i--;
	CHECK(*i == 'e');
	i--;
	CHECK(*i == 'H');
	i--;
	CHECK(*i == 'H');
}

TEST_CASE("add assign")
{
	string_view v{"Hello"};
	string_view::const_iterator i = v.begin();
	CHECK(*i == 'H');
	i += 1;
	CHECK(*i == 'e');
	i += 3;
	CHECK(*i == 'o');
	i += 1;
	CHECK(*i == '\0');
	i += 5;
	CHECK(*i == '\0');
}

TEST_CASE("minus assign")
{
	string_view v{"Hello"};
	string_view::const_iterator i = v.end();
	CHECK(*i == '\0');
	i -= 1;
	CHECK(*i == 'o');
	i -= 2;
	CHECK(*i == 'l');
	i -= 2;
	CHECK(*i == 'H');
	i -= 1;
	CHECK(*i == 'H');
	i -= 1;
	CHECK(*i == 'H');
}

TEST_CASE("comparison less")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	string_view::const_iterator j = v.begin();
	CHECK_FALSE(i < j);
	++j;
	CHECK(i < j);
	CHECK(i < v.end());
	CHECK(j < v.end());
	CHECK_FALSE(v.begin() < v.begin());
	CHECK(v.begin() < v.end());
	CHECK_FALSE(v.end() < v.end());
}

TEST_CASE("comparison greater")
{
	string_view v{"Hello World"};
	string_view::const_iterator i = v.begin();
	string_view::const_iterator j = v.begin();
	CHECK_FALSE(i > j);
	++i;
	CHECK(i > j);
	CHECK_FALSE(i > v.end());
	CHECK_FALSE(j > v.end());
	CHECK_FALSE(v.begin() > v.begin());
	CHECK_FALSE(v.begin() > v.end());
	CHECK_FALSE(v.end() > v.end());
}

TEST_CASE("operator minus")
{
	string_view v{"Hello World"};

	CHECK(v.end() - v.begin() == 11);
	CHECK(v.begin() - v.end() == -11);

	string_view::const_iterator i = v.begin();
	string_view::const_iterator j = v.begin() + 3;

	CHECK(j - i == 3);
	CHECK(i - j == -3);
}

TEST_CASE("reverse iterator")
{
	std::string s = "Hello World";
	string_view v{s.c_str()};

	auto is = s.rbegin();
	auto iv = v.rbegin();

	for (;is != s.rend() && iv != v.rend(); ++is, ++iv) {
		const auto ts = *is;
		const auto tv = *iv;
		CHECK(ts == tv);
	}
}
TEST_SUITE_END();

TEST_SUITE("element access");
TEST_CASE("index operator")
{
	static const char * s = "Hello World";
	static const auto len = std::strlen(s);
	const string_view v{s};

	CHECK(v.size() == len);
	for (auto i = 0u; i < v.size(); ++i)
		CHECK(v[i] == s[i]);
}

TEST_CASE("at")
{
	static const char * s = "Hello World";
	static const auto len = std::strlen(s);
	const string_view v{s};

	CHECK(v.size() == len);
	for (auto i = 0u; i < v.size(); ++i)
		CHECK(v.at(i) == s[i]);

	CHECK_THROWS(v.at(v.size()));
	CHECK_THROWS(v.at(v.size() + 1));
}

TEST_CASE("front")
{
	static const char * s = "Hello World";
	const string_view v{s};
	CHECK(v.front() == 'H');
}

TEST_CASE("back")
{
	static const char * s = "Hello World";
	const string_view v{s};
	CHECK(v.back() == 'd');
}

TEST_CASE("data")
{
	static const char * s = "Hello World";
	const string_view v{s};
	CHECK(v.size() == 11u);
	CHECK(std::strncmp(v.data(), s, v.size()) == 0);
}
TEST_SUITE_END();

TEST_SUITE("capacity");
TEST_CASE("size")
{
	static const char * s = "Hello World";
	const string_view v{s};
	CHECK(v.size() == std::strlen(s));
}

TEST_CASE("length")
{
	static const char * s = "Hello World";
	const string_view v{s};
	CHECK(v.length() == std::strlen(s));
}

TEST_CASE("max_size")
{
	static const char * s = "Hello World";
	const string_view v{s};
	CHECK(v.max_size() == std::strlen(s));
}

TEST_CASE("empty")
{
	static const char * s = "";
	{
		const string_view v;
		CHECK(v.empty() == true);
	}
	{
		const string_view v{s};
		CHECK(v.empty() == true);
	}
}
TEST_SUITE_END();

TEST_SUITE("modifiers");
TEST_CASE("remove_prefix")
{
	string_view a{"Hello World"};

	a.remove_prefix(6);

	CHECK(a.size() == 5);
	CHECK(std::strncmp(a.data(), "World", a.size()) == 0);
}

TEST_CASE("remove_suffix")
{
	string_view a{"Hello World"};

	a.remove_suffix(6);

	CHECK(a.size() == 5);
	CHECK(std::strncmp(a.data(), "Hello", a.size()) == 0);
}

TEST_CASE("swap")
{
	string_view a{"Hello"};
	string_view b{"World!"};

	a.swap(b);

	CHECK(a.size() == 6);
	CHECK(std::strncmp(a.data(), "World!", a.size()) == 0);
	CHECK(b.size() == 5);
	CHECK(std::strncmp(b.data(), "Hello", b.size()) == 0);
}
TEST_SUITE_END();

TEST_SUITE("operations");
TEST_CASE("copy out_of_range")
{
	const char * s = "Hello World";
	string_view v{s};
	char buf[20];
	CHECK_THROWS(v.copy(buf, 5, 20));
}

TEST_CASE("copy beginning")
{
	const char * s = "Hello World";
	string_view v{s};
	char buf[20];

	const auto n = v.copy(buf, 5, 0);
	CHECK(n == 5);
	CHECK(std::strncmp(buf, s, 5) == 0);
}

TEST_CASE("copy past the end")
{
	const char * s = "Hello World";
	string_view v{s};
	char buf[20];

	const auto n = v.copy(buf, 5, 8);
	CHECK(n == 3);
	CHECK(std::strncmp(buf, s + 8, 3) == 0);
}

TEST_CASE("substr complete")
{
	const char * s = "Hello World";
	string_view v{s};
	string_view a = v.substr();
	CHECK(a.size() == v.size());
	CHECK(std::strncmp(a.data(), v.data(), a.size()) == 0);
}

TEST_CASE("substr beginning")
{
	const char * s = "Hello World";
	string_view v{s};
	string_view a = v.substr(0, 5);
	CHECK(a.size() == 5u);
	CHECK(std::strncmp(a.data(), "Hello", a.size()) == 0);
}

TEST_CASE("substr middle")
{
	const char * s = "Hello World";
	string_view v{s};
	string_view a = v.substr(2, 6);
	CHECK(a.size() == 6u);
	CHECK(std::strncmp(a.data(), "llo Wo", a.size()) == 0);
}

TEST_CASE("substr past the end")
{
	const char * s = "Hello World";
	string_view v{s};
	string_view a = v.substr(6, 8);
	CHECK(a.size() == 5u);
	CHECK(std::strncmp(a.data(), "World", a.size()) == 0);
}

TEST_CASE("compare to string_view")
{
	string_view a{"Hello World"};
	string_view b{"Hello Worl"};
	string_view c{"Hello Xorld"};
	string_view d{"Hello World"};
	string_view e{"Hello Uorld"};
	string_view f{"Hello World!"};
	std::string g{"Hello World"};

	CHECK(a.compare(a) == 0);
	CHECK(a.compare(b) > 0);
	CHECK(a.compare(c) < 0);
	CHECK(a.compare(d) == 0);
	CHECK(a.compare(e) > 0);
	CHECK(a.compare(f) < 0);
	CHECK(a.compare(string_view{g.c_str()}) == 0);
}

TEST_CASE("find string_view")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"fox"};
	CHECK(haystack.find(needle) == 16u);
}

TEST_CASE("find string_view not from beginning")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"fox"};
	CHECK(haystack.find(needle, 10) == 16u);
}

TEST_CASE("find string_view already at the end")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"fox"};
	CHECK(haystack.find(needle, haystack.size()) == string_view::npos);
}

TEST_CASE("find string_view not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"Fox"};
	CHECK(haystack.find(needle) == string_view::npos);
}

TEST_CASE("find string_view empty")
{
	const string_view haystack{""};
	const string_view needle{"Fox"};
	CHECK(haystack.find(needle) == string_view::npos);
}

TEST_CASE("find character")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find('q') == 4u);
}

TEST_CASE("find character not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find('Q') == string_view::npos);
}

TEST_CASE("find character empty")
{
	const string_view haystack{""};
	CHECK(haystack.find('Q') == string_view::npos);
}

TEST_CASE("find character not from beginning")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find('y', 10) == 38u);
}

TEST_CASE("find character already at the end")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find('y', haystack.size()) == string_view::npos);
}

TEST_CASE("rfind string_view")
{
	const string_view haystack{"foo bar foo bar"};
	const string_view needle{"foo"};
	CHECK(haystack.rfind(needle) == 8u);
}

TEST_CASE("rfind string_view empty")
{
	const string_view haystack{""};
	const string_view needle{"foo"};
	CHECK(haystack.rfind(needle) == string_view::npos);
}

TEST_CASE("rfind string_view not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"Fox"};
	CHECK(haystack.rfind(needle) == string_view::npos);
}

TEST_CASE("rfind string_view with position")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"fox"};
	CHECK(haystack.rfind(needle, 30) == 16u);
}

TEST_CASE("rfind string_view zero position")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"fox"};
	CHECK(haystack.rfind(needle, 0) == string_view::npos);
}

TEST_CASE("rfind string_view with position to large")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	const string_view needle{"fox"};
	CHECK(haystack.rfind(needle, haystack.size() + 1) == 16u);
}

TEST_CASE("rfind character")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.rfind('q') == 4u);
}

TEST_CASE("rfind character not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.rfind('Q') == string_view::npos);
}

TEST_CASE("find_first_of string_view")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_first_of(string_view{"fox"}) == 12u);
	CHECK(haystack.find_first_of(string_view{"fx"}) == 16u);
	CHECK(haystack.find_first_of(string_view{"xyz"}) == 18u);
	CHECK(haystack.find_first_of(string_view{"q"}) == 4u);
}

TEST_CASE("find_first_of string_view not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_first_of(string_view{"FOX"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"FX"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"XYZ"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"Q"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{""}) == string_view::npos);
}

TEST_CASE("find_first_of string_view empty")
{
	const string_view haystack{""};
	CHECK(haystack.find_first_of(string_view{"fox"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"fx"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"xyz"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"q"}) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{""}) == string_view::npos);
}

TEST_CASE("find_first_of string_view not from the beginning")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_first_of(string_view{"fox"}, 10) == 12u);
	CHECK(haystack.find_first_of(string_view{"fx"}, 10) == 16u);
	CHECK(haystack.find_first_of(string_view{"xyz"}, 10) == 18u);
	CHECK(haystack.find_first_of(string_view{"q"}, 2) == 4u);
}

TEST_CASE("find_first_of string_view already at the end")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_first_of(string_view{"fox"}, haystack.size()) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"fx"}, haystack.size()) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"xyz"}, haystack.size()) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{"q"}, haystack.size()) == string_view::npos);
	CHECK(haystack.find_first_of(string_view{""}, haystack.size()) == string_view::npos);
}

TEST_CASE("find_last_of string_view")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_last_of(string_view{"fox"}) == 41u);
	CHECK(haystack.find_last_of(string_view{"fx"}) == 18u);
	CHECK(haystack.find_last_of(string_view{"xyz"}) == 38u);
	CHECK(haystack.find_last_of(string_view{"q"}) == 4u);
}

TEST_CASE("find_last_of string_view not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_last_of(string_view{"FOX"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"FX"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"XYZ"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"Q"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{""}) == string_view::npos);
}

TEST_CASE("find_last_of string_view not from the beginning")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_last_of(string_view{"fox"}, 20) == 18u);
	CHECK(haystack.find_last_of(string_view{"fx"}, 18) == 16u);
	CHECK(haystack.find_last_of(string_view{"xyz"}, 10) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"q"}, 10) == 4u);
}

TEST_CASE("find_last_of string_view empty")
{
	const string_view haystack{""};
	CHECK(haystack.find_last_of(string_view{"fox"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"fx"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"xyz"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"q"}) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{""}) == string_view::npos);
}

TEST_CASE("find_last_of string_view already at the rend")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_last_of(string_view{"fox"}, 0) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"fx"}, 0) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"xyz"}, 0) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{"q"}, 0) == string_view::npos);
	CHECK(haystack.find_last_of(string_view{""}, 0) == string_view::npos);
}

TEST_CASE("find_first_not_of string_view")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_first_not_of(string_view{"the quick"}) == 10u);
	CHECK(haystack.find_first_not_of(string_view{"thequickbrown "}) == 16u);
	CHECK(haystack.find_first_not_of(string_view{"q"}) == 0u);
}

TEST_CASE("find_first_not_of string_view not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_first_not_of(string_view{"THEQUICK"}) == 0u);
	CHECK(haystack.find_first_not_of(string_view{"THEQUICKBROWN"}) == 0u);
	CHECK(haystack.find_first_not_of(string_view{"Q"}) == 0u);
	CHECK(haystack.find_first_not_of(string_view{"abcdefghijklmnopqrstuvwxyz "})
		== string_view::npos);
}

TEST_CASE("find_first_not_of string_view empty")
{
	const string_view haystack{""};
	CHECK(haystack.find_first_not_of(string_view{"the quick"}) == string_view::npos);
	CHECK(haystack.find_first_not_of(string_view{"thequickbrown "}) == string_view::npos);
	CHECK(haystack.find_first_not_of(string_view{"q"}) == string_view::npos);
}

TEST_CASE("find_last_not_of string_view")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_last_not_of(string_view{"lazy dog"}) == 33u);
	CHECK(haystack.find_last_not_of(string_view{"doglazythe "}) == 29u);
	CHECK(haystack.find_last_not_of(string_view{"q"}) == 42u);
	CHECK(haystack.find_last_not_of(string_view{"g"}) == 41u);
}

TEST_CASE("find_last_not_of string_view not found")
{
	const string_view haystack{"the quick brown fox jumps over the lazy dog"};
	CHECK(haystack.find_last_not_of(string_view{"THEQUICK"}) == 42u);
	CHECK(haystack.find_last_not_of(string_view{"THEQUICKBROWN"}) == 42u);
	CHECK(haystack.find_last_not_of(string_view{"Q"}) == 42u);
	CHECK(haystack.find_last_not_of(string_view{"abcdefghijklmnopqrstuvwxyz "})
		== string_view::npos);
}

TEST_CASE("find_last_not_of string_view empty")
{
	const string_view haystack{""};
	CHECK(haystack.find_last_not_of(string_view{"the quick"}) == string_view::npos);
	CHECK(haystack.find_last_not_of(string_view{"thequickbrown "}) == string_view::npos);
	CHECK(haystack.find_last_not_of(string_view{"q"}) == string_view::npos);
}
TEST_SUITE_END();

TEST_SUITE("non-member functions");
TEST_CASE("operator equal")
{
	string_view a{"foobar"};
	string_view b{"foobar"};
	string_view c{"Foobar"};
	string_view d{a};

	CHECK(a == a);
	CHECK(a == b);
	CHECK_FALSE(a == c);
	CHECK(a == d);

	CHECK(b == a);
	CHECK(b == b);
	CHECK_FALSE(b == c);
	CHECK(b == d);

	CHECK_FALSE(c == a);
	CHECK_FALSE(c == b);
	CHECK(c == c);
	CHECK_FALSE(c == d);

	CHECK(d == a);
	CHECK(d == b);
	CHECK_FALSE(d == c);
	CHECK(d == d);
}
TEST_SUITE_END();

