#pragma once

template<class Callable>
class [[nodiscard]] Finally final {
public:
    explicit Finally(Callable&& callable) : callable(std::forward<Callable>(callable))
    {}
    
    ~Finally() { callable(); }

private:
    Callable callable;
};
