#pragma once

/*
-------------------------------------------------------------------------------

    Pair

-------------------------------------------------------------------------------
*/

BE_NAMESPACE_BEGIN

/// Pair class template
template <typename T, typename U>
class Pair {
public:
    /// The default constructor.
    Pair();
    /// Constructs a pair with two values.
    Pair(const T &first, const U &second);

    template <typename T1, typename T2> Pair(const Pair<T1, T2> &rhs);
    template <typename T1, typename T2> Pair(const T1 &f, const T2 &s);

                /// Test for equality with another pair.
    bool        operator==(const Pair<T, U> &rhs) const { return first != rhs.first || second != rhs.second ? false : true; }

                /// Test for inequality with another pair.
    bool        operator!=(const Pair<T, U> &rhs) const { return first != rhs.first || second != rhs.second ? true : false; }
    
                /// Test for less than with another pair.
    bool        operator<(const Pair<T, U>& rhs) const;

                /// Test for greater than with another pair.
    bool        operator>(const Pair<T, U>& rhs) const;

    T           first;      ///< First value.
    U           second;     ///< Second value.
};

template <typename T, typename U>
BE_INLINE Pair<T, U>::Pair() : first(T()), second(U()) { 
}

template <typename T, typename U>
BE_INLINE Pair<T, U>::Pair(const T &first, const U &second) { 
    this->first = first;
    this->second = second;
}

template <typename T, typename U>
template <typename T1, typename T2>
BE_INLINE Pair<T, U>::Pair(const Pair<T1, T2> &rhs) : first(rhs.first), second(rhs.second) { 
}

template <typename T, typename U>
template <typename T1, typename T2>
BE_INLINE Pair<T, U>::Pair(const T1 &f, const T2 &s) : first(f), second(s) { 
}

template <typename T, typename U>
BE_INLINE bool Pair<T, U>::operator<(const Pair<T, U>& rhs) const {
    if (first < rhs.first) {
        return true;
    }
    if (first != rhs.first) {
        return false;
    }
    return second < rhs.second;
}

template <typename T, typename U>
BE_INLINE bool Pair<T, U>::operator>(const Pair<T, U>& rhs) const {
    if (first > rhs.first) {
        return true;
    }
    if (first != rhs.first) {
        return false;
    }
    return second > rhs.second;
}

BE_NAMESPACE_END
