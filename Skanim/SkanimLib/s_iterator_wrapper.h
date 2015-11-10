#pragma once

namespace Skanim
{
    /** The base iterator wrapper.
     *  @tparam T Container type.
     *  @tparam IteratorType T::iterator or T::const_iterator
     *  @tparam ValType T::mapped_type in case of a map, T::value_type for 
     *  others.
     */
    template <typename T, typename IteratorType, typename ValType>
    class IteratorWrapper
    {
    public:
        /** Default constructor is not allowed.
         */
        IteratorWrapper() = delete;

        /** Construct a iterator from begin and end iterators.
         */
        IteratorWrapper(IteratorType begin, IteratorType end) noexcept
            : m_begin(begin), m_current(begin), m_end(end)
        {}

        /** Check if there is a next elements.
         */
        bool hasNext() const
        {
            return m_current != m_end;
        }

        /** Move the iterator to the next element.
         */
        void moveNext()
        {
            ++m_current;
        }

        /** Get the begin iterator.
         */
        const IteratorType &begin() const
        {
            return m_begin;
        }

        /** Get the end iterator.
         */
        const IteratorType &end() const
        {
            return m_end;
        }

        /** Get the current iterator.
         */
        IteratorType &current() 
        {
            return m_current;
        }

        typedef ValType ValueType;
        typedef ValType *PointerType;

    protected:

        IteratorType m_begin;
        IteratorType m_current;
        IteratorType m_end;

    };

    /** Iterator for containers except std::map
     *  @tparam T Any container type except std::map
     */
    template <typename T, typename IteratorType>
    using VectorIteratorWrapper = IteratorWrapper<T, IteratorType, typename T::value_type>;

    template <typename T>
    using VectorIterator = VectorIteratorWrapper<T, typename T::iterator>;

    template <typename T>
    using VectorConstIterator = VectorIteratorWrapper<T, typename T::const_iterator>;

    /** Iterator for containers std::map
     *  @tparam T Any container type
     */
    template <typename T, typename IteratorType>
    using MapIteratorWrapper = IteratorWrapper<T, IteratorType, typename T::mapped_type>;

    template <typename T>
    using MapIterator = MapIteratorWrapper<T, typename T::iterator>;

    template <typename T>
    using MapConstIterator = MapIteratorWrapper<T, typename T::const_iterator>;
};
