#pragma once
#include "mvector.hpp"
#include "mdllist.hpp"

// implemented using dllist
namespace mlib
{
    template <typename T, typename Container = mlib::dllist<T>>
    class stack
    {
        Container m_ul_container;
        using value_type = Container::value_type;
        using reference = Container::reference;
        using const_reference = Container::const_reference;
        using size_type = Container::size_type;

    public:
        stack() {};
        void push(const_reference _val_)
        {
            m_ul_container.append_back(_val_);
        };
        void pop()
        {
            if (m_ul_container.size() == 0)
            {
                return;
            }
            m_ul_container.pop_back();
        };

        Container &get_container() noexcept
        {
            return m_ul_container;
        };

        [[nodiscard]] reference top() const noexcept
        {

            return m_ul_container.back();
        }

        [[nodiscard]] bool empty() const noexcept
        {
            return m_ul_container.size() == 0;
        };

        [[nodiscard]] size_type size() const noexcept
        {
            return m_ul_container.size();
        };

        ~stack() {};
    };
}
