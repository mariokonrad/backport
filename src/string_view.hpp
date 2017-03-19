#ifndef BACKPORT__CPP11__STRING_VIEW__HPP
#define BACKPORT__CPP11__STRING_VIEW__HPP

/// MIT License
///
/// Copyright (c) 2017 Mario Konrad <mario.konrad@gmx.net>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.

#include <algorithm>
#include <iterator>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <cassert>

namespace backport
{
namespace cpp11
{
///
/// Backport of `std::string_view` for C++11.
///
/// Omitting helper classes for hashing.
///
template <class CharT, class Traits = std::char_traits<CharT>> class basic_string_view
{
	static_assert(std::is_same<CharT, typename Traits::char_type>::value, "");

public:
	using traits_type = Traits;
	using value_type = CharT;
	using pointer = CharT *;
	using const_pointer = const CharT *;
	using reference = CharT &;
	using const_reference = const CharT &;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;

	class const_iterator
	{
		friend class basic_string_view;

	public:
		using difference_type = basic_string_view::difference_type;
		using value_type = basic_string_view::value_type;
		using pointer = basic_string_view::const_pointer;
		using reference = basic_string_view::const_reference;
		using iterator_category = std::random_access_iterator_tag;

	public: // construction
		constexpr const_iterator() noexcept = default;
		constexpr const_iterator(const const_iterator &) noexcept = default;
		constexpr const_iterator(const_iterator &&) noexcept = default;

	public: // assignment
		/*constexpr*/ const_iterator & operator=(const const_iterator & other) noexcept
		{
			const_iterator it{other};
			swap(it);
			return *this;
		}

		/*constexpr*/ const_iterator & operator=(const_iterator && other) noexcept
		{
			view_ = std::move(other.view_);
			pos_ = std::move(other.pos_);
			return *this;
		}

	public: // comparison operators
		constexpr bool operator==(const const_iterator & other) const
		{
			return (this == &other) || ((view_ == other.view_) && (pos_ == other.pos_));
		}

		constexpr bool operator!=(const_iterator other) const { return !(*this == other); }

		constexpr bool operator<(const const_iterator & other) const
		{
			return (this != &other) && (view_ == other.view_) && (pos_ < other.pos_);
		}

		constexpr bool operator>(const const_iterator & other) const
		{
			return (this != &other) && (view_ == other.view_) && (pos_ > other.pos_);
		}

		constexpr bool operator<=(const const_iterator & other) const
		{
			return !(*this > other);
		}

		constexpr bool operator>=(const const_iterator & other) const
		{
			return !(*this < other);
		}

	public: // modifiers
		/*constexpr*/ const_iterator & operator++()
		{
			if (pos_ < view_->size())
				++pos_;
			return *this;
		}

		/*constexpr*/ const_iterator operator++(int)
		{
			const_iterator i{*this};
			++(*this);
			return i;
		}

		/*constexpr*/ const_iterator & operator--()
		{
			if (pos_ > 0u)
				--pos_;
			return *this;
		}

		/*constexpr*/ const_iterator operator--(int)
		{
			const_iterator i{*this};
			--(*this);
			return i;
		}

		/*constexpr*/ const_iterator & operator+=(size_type n)
		{
			pos_ = std::min(view_->size(), pos_ + n);
			return *this;
		}

		/*constexpr*/ const_iterator & operator-=(size_type n)
		{
			pos_ -= n;
			if (pos_ > view_->size()) // underflow
				pos_ = 0u;
			return *this;
		}

		friend /*constexpr*/ const_iterator operator+(const const_iterator & it, size_type n)
		{
			return const_iterator{it} += n;
		}

		friend /*constexpr*/ const_iterator operator+(size_type n, const const_iterator & it)
		{
			return const_iterator{it} += n;
		}

		friend /*constexpr*/ const_iterator operator-(const const_iterator & it, size_type n)
		{
			return const_iterator{it} -= n;
		}

		friend /*constexpr*/ const_iterator operator-(size_type n, const const_iterator & it)
		{
			return const_iterator{it} -= n;
		}

		friend /*constexpr*/ difference_type operator-(
			const const_iterator & a, const const_iterator & b)
		{
			difference_type d = a.pos_;
			d -= b.pos_;
			return d;
		}

		void swap(const_iterator & other)
		{
			std::swap(view_, other.view_);
			std::swap(pos_, other.pos_);
		}

	public: // access
		constexpr const_reference operator*() const { return *(view_->data() + pos_); }

		constexpr const basic_string_view * operator->() const { return view_; }

		constexpr value_type operator[](size_type n) const
		{
			return *(view_->data() + pos_ + n);
		}

	private: // construction
		const_iterator(basic_string_view * view, size_type pos)
			: view_(view)
			, pos_(pos)
		{
			assert(view_);
		}

	private:
		basic_string_view * view_ = nullptr;
		size_type pos_ = npos;
	};

	using iterator = const_iterator;

	using const_reverse_iterator = std::reverse_iterator<const_iterator>;
	using reverse_iterator = const_reverse_iterator;

public: // constants
	static constexpr size_type npos = std::numeric_limits<size_type>::max();

public: // construction
	constexpr basic_string_view() noexcept = default;

	constexpr basic_string_view(const basic_string_view &) noexcept = default;

	constexpr basic_string_view(const CharT * s, size_type count) noexcept
		: ptr_(s)
		, size_(count)
	{
	}

	/*constexpr*/ basic_string_view(const CharT * s)
		: ptr_(s)
		, size_(traits_type::length(s))
	{
	}

public: // assignment
	basic_string_view & operator=(const basic_string_view &) = default;

public: // iterators
	/*constexpr*/ const_iterator begin() const
	{
		return const_iterator{const_cast<basic_string_view *>(this), 0u};
	}

	/*constexpr*/ const_iterator cbegin() const { return begin(); }

	/*constexpr*/ const_iterator end() const
	{
		return const_iterator{const_cast<basic_string_view *>(this), size()};
	}

	/*constexpr*/ const_iterator cend() const { return end(); }

	constexpr const_reverse_iterator rbegin() const { return const_reverse_iterator{end()}; }

	constexpr const_reverse_iterator crbegin() const { return const_reverse_iterator{cend()}; }

	constexpr const_reverse_iterator rend() const { return const_reverse_iterator{begin()}; }

	constexpr const_reverse_iterator crend() const { return const_reverse_iterator{cbegin()}; }

public: // element access
	constexpr const_reference operator[](size_type i) const { return ptr_[i]; }

	/*constexpr*/ const_reference at(size_type i) const
	{
		if (i >= size_)
			throw std::out_of_range("at");
		return ptr_[i];
	}

	constexpr const_reference front() const { return ptr_[0]; }

	constexpr const_reference back() const { return ptr_[size_ - 1]; }

	constexpr const_pointer data() const noexcept { return ptr_; }

public: // capacity
	constexpr size_type size() const noexcept { return size_; }

	constexpr size_type length() const noexcept { return size_; }

	constexpr size_type max_size() const noexcept { return size_; }

	constexpr bool empty() const noexcept { return !ptr_ || size_ == 0; }

public: // modifiers
	/*constexpr*/ void remove_prefix(size_type n)
	{
		ptr_ += n;
		size_ -= n;
	}

	/*constexpr*/ void remove_suffix(size_type n) { size_ -= n; }

	/*constexpr*/ void swap(basic_string_view & v) noexcept
	{
		std::swap(ptr_, v.ptr_);
		std::swap(size_, v.size_);
	}

public: // operations
	size_type copy(CharT * dest, size_type count, size_type pos = 0) const
	{
		if (pos >= size())
			throw std::out_of_range{"copy"};
		const size_type n = std::min(count, size_ - pos);
		traits_type::copy(dest, ptr_ + pos, n);
		return n;
	}

	constexpr basic_string_view substr(size_type pos = 0, size_type count = npos) const
	{
		return {ptr_ + pos, (size_ - pos) < count ? (size_ - pos) : count};
	}

	/*constexpr*/ int compare(basic_string_view v) const noexcept
	{
		assert(ptr_);
		assert(v.ptr_);

		// optimizations for comparisons to itself
		if ((this == &v) || ((ptr_ == v.ptr_) && (size_ == v.size_)))
			return 0;

		// compare lexicographically
		const auto result = traits_type::compare(data(), v.data(), std::min(size(), v.size()));
		if (result < 0)
			return -1;
		if (result > 0)
			return +1;
		if (size() < v.size())
			return -1;
		if (size() > v.size())
			return +1;
		return 0;
	}

	/*constexpr*/ int compare(size_type pos1, size_type count1, basic_string_view v) const
	{
		return substr(pos1, count1).compare(v);
	}

	/*constexpr*/ int compare(size_type pos1, size_type count1, basic_string_view v,
		size_type pos2, size_type count2) const
	{
		return substr(pos1, count1).compare(v.substr(pos2, count2));
	}

	/*constexpr*/ int compare(const CharT * s) const { return compare(basic_string_view{s}); }

	/*constexpr*/ int compare(size_type pos1, size_type count1, const CharT * s) const
	{
		return substr(pos1, count1).compare(basic_string_view{s});
	}

	/*constexpr*/ int compare(
		size_type pos1, size_type count1, const CharT * s, size_type count2) const
	{
		return substr(pos1, count1).compare(basic_string_view{s, count2});
	}

	/*constexpr*/ size_type find(basic_string_view v, size_type pos = 0) const noexcept
	{
		const auto i = std::search(begin() + pos, end(), v.begin(), v.end(), traits_type::eq);
		return (i == end()) ? npos : (i - begin());
	}

	/*constexpr*/ size_type find(CharT c, size_type pos = 0) const noexcept
	{
		const auto i = traits_type::find(ptr_ + pos, size_ - pos, c);
		return i ? (i - ptr_) : npos;
	}

	/*constexpr*/ size_type find(const CharT * s, size_type pos, size_type count) const
	{
		return find(basic_string_view{s, count}, pos);
	}

	/*constexpr*/ size_type find(const CharT * s, size_type pos = 0) const
	{
		return find(basic_string_view{s}, pos);
	}

	/*constexpr*/ size_type rfind(basic_string_view v, size_type pos = npos) const noexcept
	{
		pos = (pos >= size()) ? 0 : (size() - pos); // offset from end of data
		const auto i
			= std::search(rbegin() + pos, rend(), v.rbegin(), v.rend(), traits_type::eq);
		return (i == rend()) ? npos : (size() - v.size() - std::distance(rbegin(), i));
	}

	/*constexpr*/ size_type rfind(CharT c, size_type pos = npos) const noexcept
	{
		return rfind(basic_string_view{&c, 1}, pos);
	}

	/*constexpr*/ size_type rfind(const CharT * s, size_type pos, size_type count) const
	{
		return rfind(basic_string_view{s, count}, pos);
	}

	/*constexpr*/ size_type rfind(const CharT * s, size_type pos = npos) const
	{
		return rfind(basic_string_view{s}, pos);
	}

	/*constexpr*/ size_type find_first_of(basic_string_view v, size_type pos = 0) const noexcept
	{
		const auto i
			= std::find_first_of(begin() + pos, end(), v.begin(), v.end(), traits_type::eq);
		return (i == end()) ? npos : (i - begin());
	}

	/*constexpr*/ size_type find_first_of(CharT c, size_type pos = 0) const noexcept
	{
		return find(c, pos);
	}

	/*constexpr*/ size_type find_first_of(const CharT * s, size_type pos, size_type count) const
	{
		return find_first_of(basic_string_view{s, count}, pos);
	}

	/*constexpr*/ size_type find_first_of(const CharT * s, size_type pos = 0) const
	{
		return find_first_of(basic_string_view{s}, pos);
	}

	/*constexpr*/ size_type find_last_of(basic_string_view v, size_type pos = npos) const
		noexcept
	{
		pos = (pos >= size()) ? 0 : (size() - pos); // offset from end of data
		const auto i
			= std::find_first_of(rbegin() + pos, rend(), v.begin(), v.end(), traits_type::eq);
		return (i == rend()) ? npos : (size() - std::distance(rbegin(), i) - 1);
	}

	/*constexpr*/ size_type find_last_of(CharT c, size_type pos = npos) const noexcept
	{
		return find_last_of(basic_string_view{&c, 1}, pos);
	}

	/*constexpr*/ size_type find_last_of(const CharT * s, size_type pos, size_type count) const
	{
		return find_last_of(basic_string_view{s, count}, pos);
	}

	/*constexpr*/ size_type find_last_of(const CharT * s, size_type pos = npos) const
	{
		return find_last_of(basic_string_view{s}, pos);
	}

	/*constexpr*/ size_type find_first_not_of(basic_string_view v, size_type pos = 0) const
		noexcept
	{
		const auto i = std::find_if(begin() + pos, end(),
			[v](typename traits_type::char_type c) { return v.find(c) == npos; });
		return (i == end()) ? npos : (i - begin());
	}

	/*constexpr*/ size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept
	{
		return find_first_not_of(basic_string_view{&c, 1}, pos);
	}

	/*constexpr*/ size_type find_first_not_of(
		const CharT * s, size_type pos, size_type count) const
	{
		return find_first_not_of(basic_string_view{s, count}, pos);
	}

	/*constexpr*/ size_type find_first_not_of(const CharT * s, size_type pos = 0) const
	{
		return find_first_not_of(basic_string_view{s}, pos);
	}

	/*constexpr*/ size_type find_last_not_of(basic_string_view v, size_type pos = npos) const
		noexcept
	{
		pos = (pos >= size()) ? 0 : (size() - pos); // offset from end of data
		const auto i = std::find_if(rbegin() + pos, rend(),
			[v](typename traits_type::char_type c) { return v.find(c) == npos; });
		return (i == rend()) ? npos : (size() - std::distance(rbegin(), i) - 1);
	}

	/*constexpr*/ size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept
	{
		return find_last_not_of(basic_string_view{&c, 1}, pos);
	}

	/*constexpr*/ size_type find_last_not_of(
		const CharT * s, size_type pos, size_type count) const
	{
		return find_last_not_of(basic_string_view{s, count}, pos);
	}

	/*constexpr*/ size_type find_last_not_of(const CharT * s, size_type pos = npos) const
	{
		return find_last_not_of(basic_string_view{s}, pos);
	}

private:
	const_pointer ptr_ = nullptr;
	size_type size_ = 0u;
};

template <class CharT, class Traits>
constexpr
	typename basic_string_view<CharT, Traits>::size_type basic_string_view<CharT, Traits>::npos;

// comparison operators

template <class CharT, class Traits = std::char_traits<CharT>>
constexpr bool operator==(const basic_string_view<CharT, Traits> & a,
	const basic_string_view<CharT, Traits> & b) noexcept
{
	return a.compare(b) == 0;
}

template <class CharT, class Traits = std::char_traits<CharT>>
constexpr bool operator!=(const basic_string_view<CharT, Traits> & a,
	const basic_string_view<CharT, Traits> & b) noexcept
{
	return a.compare(b) != 0;
}

template <class CharT, class Traits = std::char_traits<CharT>>
constexpr bool operator<(const basic_string_view<CharT, Traits> & a,
	const basic_string_view<CharT, Traits> & b) noexcept
{
	return a.compare(b) < 0;
}

template <class CharT, class Traits = std::char_traits<CharT>>
constexpr bool operator>(const basic_string_view<CharT, Traits> & a,
	const basic_string_view<CharT, Traits> & b) noexcept
{
	return a.compare(b) > 0;
}

template <class CharT, class Traits = std::char_traits<CharT>>
constexpr bool operator<=(const basic_string_view<CharT, Traits> & a,
	const basic_string_view<CharT, Traits> & b) noexcept
{
	return !(a > b);
}

template <class CharT, class Traits = std::char_traits<CharT>>
constexpr bool operator>=(const basic_string_view<CharT, Traits> & a,
	const basic_string_view<CharT, Traits> & b) noexcept
{
	return !(a < b);
}

// string
template <class CharT, class Traits = std::char_traits<CharT>>
constexpr std::basic_string<CharT, Traits> to_string(basic_string_view<CharT, Traits> v)
{
	return std::basic_string<CharT, Traits>{v.begin(), v.end()};
}

// stream operators
template <class CharT, class Traits>
std::basic_ostream<CharT, Traits> & operator<<(
	std::basic_ostream<CharT, Traits> & os, basic_string_view<CharT, Traits> v)
{
	return os << to_string(v);
}

// default types
using string_view = basic_string_view<char>;
}
}

#endif
