#include <iostream>
#include <tuple>

auto list = [](auto ...xs) {
    return [=](auto access) { return access(xs...); };
};

auto length = [](auto xs) {
    return xs([](auto ...z) { return sizeof...(z); });
};

namespace internal 
{
  template <int N, typename... T>
  struct get;

  template <typename T0, typename... T>
  struct get<0, T0, T...> 
  {
    T0 value;
    get(T0 t0, T... t)
    : value(t0)
    {}
  };

  template <int N, typename T0, typename... T>
  struct get<N, T0, T...> : public get<N-1, T...>
  {
    get(T0 t0, T... t)
    : get<N-1, T...> (t...)
    {}
  };
} //!internal

template <int N, typename L>
auto get(L && xs)
{
  return xs([] (auto ... list) { return internal::get<N, decltype(list)...>(list...).value;});
}


int main()
{
  auto tuple_int = list(int(1), short(2), char(3));
  std::cout << sizeof(tuple_int) << " / " << sizeof(std::tuple<int, short, char>) << std::endl;

  auto tuple = list(1, '2', "3");
  std::cout << length(tuple) << std::endl;
  std::cout << get<1>(tuple) << std::endl;
  return 0;
}
