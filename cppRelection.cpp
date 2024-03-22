#include <iostream>
#include <string>

using namespace std;
#define __GCC__ 0

#if __GCC__
#define LOG(x)	(cout << __FILE__ << ':' << __LINE__<<" : " << __PRETTY_FUNCTION__ << ": " << (x) << endl)
#else
#define LOG(x)	(cout << __FILE__ << ':' << __LINE__<<" : " << __FUNCSIG__ << ": " << (x) << endl)
#endif // __GCC__

void func() {
	LOG("int the function()");
}

/// Compilation Relfection.
template <class T>
string get_type_name()
{
#if __GCC__
	return __PRETTY_FUNCTION__;
#else 
	string s =  __FUNCSIG__;
	auto pos = s.find("name<");
	pos += 5;
	auto pos2 = s.find_first_of(">", pos);
	return s.substr(pos, pos2 - pos);

#endif // !
}

namespace myutils {
	struct MyClass;
}


template <typename T, T N>
string get_int_name() {
#if __GCC__
	return __PRETTY_FUNCTION__;
#else 
	string s = __FUNCSIG__;
	auto pos = s.find("name<");
	pos += 5;
	auto pos2 = s.find_first_of(">", pos);
	return s.substr(pos, pos2 - pos);
	//return s;
#endif // !

}


enum Color {
	RED=1, GREEN=2, BLUE=3,
};


template<int N>
struct int_constant
{
	static constexpr int value = N;
};

template<int BEG, int END, typename F>
void static_for(F const & func)
{
	if constexpr (BEG == END) return;
	else
	{
		func(int_constant<BEG>());
		static_for<BEG + 1, END>(func);
	}
}

template<class T>
string get_int_name_dynamic(T n)
{
	// if (n == 1) return get_int_name<T, T(1)>();
	string ret;
	static_for<0, 256>([&](auto ic) {
		constexpr auto i = ic.value;
		if (n == (T)i) { ret = get_int_name<T, T(i)>(); }
		});
	return ret;
}

int main()
{
	// test to print the function signature.
	func();
	LOG("hellow");
	//
	LOG("Try the compilation relection...\n");
	typedef int T;
	//case 1
	cout << get_type_name<uint32_t>() << endl;
	cout << get_type_name<myutils::MyClass>() << endl;
	
	// case 2:
	LOG("Get the value..");
	cout << get_int_name<int, 100>() << endl;

	// case3:
	LOG("case3: enumeration.");
	cout << get_int_name<Color, RED>() << endl;
	cout << get_int_name<Color, (Color)4>() << endl;
	
	// case 4: get int name dynamiclly
	LOG("dynamic relection");
	Color c = GREEN;	
	cout << get_int_name_dynamic(c) << endl;

	return 0;
}
