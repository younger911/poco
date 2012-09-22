//
// Nullable.h
//
// $Id: //poco/1.4/Foundation/include/Poco/Nullable.h#1 $
//
// Library: Foundation
// Package: Core
// Module:  Nullable
//
// Definition of the Nullable template class.
//
// Copyright (c) 2010, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_Nullable_INCLUDED
#define Foundation_Nullable_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include <algorithm>
#include <iostream>


namespace Poco {


enum NullType
{
	NULL_GENERIC = 0
};


template <typename C>
class Nullable
	/// Nullable is a simple wrapper class for value types
	/// that allows objects or native type variables 
	/// to have "null" value.
	///
	/// The class is useful for passing parameters to functions
	/// when parameters are optional and no default values 
	/// should be used or when a non-assigned state is needed,
	/// such as in e.g. fetching null values from database.
	///
	/// A Nullable can be default constructed. In this case, 
	/// the Nullable will have a Null value and isNull() will
	/// return true. Calling value() (without default value) on
	/// a Null object will throw a NullValueException.
	///
	/// A Nullable can also be constructed from a value.
	/// It is possible to assign a value to a Nullable, and
	/// to reset a Nullable to contain a Null value by calling
	/// clear().
	///
	/// For use with Nullable, the value type should support
	/// default construction.
{
public:
	Nullable(): 
		/// Creates an empty Nullable.
		_value(),
		_isNull(true)
	{
	}

	Nullable(const NullType&): 
		/// Creates an empty Nullable.
		_value(),
		_isNull(true)
	{
	}

	Nullable(const C& value): 
		/// Creates a Nullable with the given value.
		_value(value), 
		_isNull(false)
	{
	}
	
	Nullable(const Nullable& other):
		/// Creates a Nullable by copying another one.
		_value(other._value),
		_isNull(other._isNull)
	{
	}

	~Nullable()
		/// Destroys the Nullable.
	{
	}

	Nullable& assign(const C& value)
		/// Assigns a value to the Nullable.
	{
		_value  = value;
		_isNull = false;
		return *this;
	}
	
	Nullable& assign(const Nullable& other)
		/// Assigns another Nullable.
	{
		Nullable tmp(other);
		swap(tmp);
		return *this;
	}
	
	Nullable& assign(NullType)
		/// Sets value to null.
	{
		_isNull = true;
		return *this;
	}
	
	Nullable& operator = (const C& value)
		/// Assigns a value to the Nullable.
	{
		return assign(value);
	}

	Nullable& operator = (const Nullable& other)
		/// Assigns another Nullable.
	{
		return assign(other);
	}

	Nullable& operator = (NullType)
		/// Assigns another Nullable.
	{
		_isNull = true;
		return *this;
	}

	void swap(Nullable& other)
		/// Swaps this Nullable with other.
	{
		std::swap(_value, other._value);
		std::swap(_isNull, other._isNull);
	}

	bool operator == (const Nullable<C>& other) const
		/// Compares two Nullables for equality
	{
		return (_isNull && other._isNull) || (_isNull == other._isNull && _value == other._value);
	}

	bool operator == (const C& value) const
		/// Compares Nullable with value for equality
	{
		return (!_isNull && _value == value);
	}

	bool operator == (const NullType&) const
		/// Compares Nullable with NullData for equality
	{
		return _isNull;
	}

	bool operator != (const C& value) const
		/// Compares Nullable with value for non equality
	{
		return !(*this == value);
	}

	bool operator != (const Nullable<C>& other) const
		/// Compares two Nullables for non equality
	{
		return !(*this == other);
	}

	bool operator != (const NullType&) const
		/// Compares with NullData for non equality
	{
		return !_isNull;
	}

	bool operator < (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is smaler than the other object's value.
		/// Null value is smaller than a non-null value.
	{
		if (_isNull && other._isNull) return false;

		if (!_isNull && !other._isNull)
			return (_value < other._value);

		if (_isNull && !other._isNull) return true;

		return false;
	}

	bool operator > (const Nullable<C>& other) const
		/// Compares two Nullable objects. Return true if this object's
		/// value is greater than the other object's value.
		/// A non-null value is greater than a null value.
	{
		return !(*this == other) && !(*this < other);
	}

	C& value()
		/// Returns the Nullable's value.
		///
		/// Throws a NullValueException if the Nullable is empty.
	{
		if (!_isNull)
			return _value;
		else
			throw NullValueException();
	}

	const C& value() const
		/// Returns the Nullable's value.
		///
		/// Throws a NullValueException if the Nullable is empty.
	{
		if (!_isNull)
			return _value;
		else
			throw NullValueException();
	}

	const C& value(const C& deflt) const
		/// Returns the Nullable's value, or the
		/// given default value if the Nullable is empty.
	{
		return _isNull ? deflt : _value;
	}

	operator C& ()
		/// Get reference to the value
	{
		return value();
	}

	operator const C& () const
		/// Get const reference to the value
	{
		return value();
	}

	operator NullType& ()
		/// Get reference to the value
	{

		return _null;
	}

	bool isNull() const
		/// Returns true iff the Nullable is empty.
	{
		return _isNull;
	}
	
	void clear()
		/// Clears the Nullable.
	{
		_isNull = true;
	}

private:
	C        _value;
	bool     _isNull;
	NullType _null;
};


template <typename C>
inline void swap(Nullable<C>& n1, Nullable<C>& n2)
{
	n1.swap(n2);
}


template <typename C>
std::ostream& operator<<(std::ostream& out, const Nullable<C>& obj) 
{
	if (!obj.isNull()) out << obj.value();
	return out;
}


template <typename C>
bool operator == (const NullType&, const Nullable<C>& n)
	/// Returns true if this Nullable is null.
{
	return n.isNull();
}


template <typename C>
bool operator != (const C& c, const Nullable<C>& n)
	/// Compares Nullable with value for non equality
{
	return !(n == c);
}


template <typename C>
bool operator == (const C& c, const Nullable<C>& n)
	/// Compares Nullable with NullData for equality
{
	return (n == c);
}


template <typename C>
bool operator != (const NullType&, const Nullable<C>& n)
	/// Returns true if this Nullable is not null.
{
	return !n.isNull();
}


} // namespace Poco


#endif // Foundation_Nullable_INCLUDED
