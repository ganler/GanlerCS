#include <type_traits>
#include <iostream>
#include <tuple>
#include <cmath>

namespace autograd1d
{

template <typename ... Layers>
class sequential
{
public:

    template <typename T>
    decltype(auto) forward(T&& inp)
    {
        std::get<0>(layers).forward(inp);
        forward_impl(std::make_index_sequence<sizeof...(Layers)-1>{});
        return std::get<sizeof...(Layers)-1>(layers).result;
    }
    decltype(auto) grad()
    {
        return grad_impl<0>(1);
    }
private:
    template <std::size_t ... Is>
    void forward_impl(std::index_sequence<Is...>)
    {
        (std::get<Is+1>(layers).forward(std::get<Is>(layers).result), ...);
    }
    template <std::size_t I, typename T>
    decltype(auto) grad_impl(T&& data)
    {
        if constexpr (I == sizeof...(Layers))
            return static_cast<std::remove_reference_t<T>>(data);
        else
            return grad_impl<I+1>(std::get<I>(layers).grad(data));
    }
    std::tuple<Layers...> layers;
};

template <typename ... Args>
sequential<Args ...> make_seq(Args&& ... args)
{
    return sequential<Args...>();
}

}

template <int I, typename T>
struct times
{
    T result;
    template <typename U>
    constexpr std::remove_reference_t<T>&
    forward(U&& input)
    {
        result = I*input;
        return result;
    }
    template <typename U>
    constexpr std::remove_reference_t<T>
    grad(U&& acc_grad)
    {
        return I * acc_grad;
    }
};

template <typename T>
struct exponential
{
    T result;
    template <typename U>
    constexpr std::remove_reference_t<T>&
    forward(U&& input)
    {
        result = std::exp(input);
        return result;
    }
    template <typename U>
    constexpr std::remove_reference_t<U>
    grad(U&& acc_grad)
    {
        return result * acc_grad;
    }
};

int main()
{
    using namespace autograd1d;
    auto graph = make_seq(
            times<2, double>(),     // 2*x
            exponential<double>()   // e^(2*x)
    );
    std::cout << graph.forward(1) << '\n';
    std::cout << graph.grad() << '\n';
}