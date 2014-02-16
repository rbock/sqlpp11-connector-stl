/*
 * Copyright (c) 2014, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_CONTAINER_COLUMN_H
#define SQLPP_CONTAINER_COLUMN_H

namespace sqlpp
{
	namespace container
	{
		template<typename Column>
			struct column_t
			{
				static_assert(::sqlpp::is_column_t<Column>::value, "Invalid argument for column_t");

				using _column_spec_t = typename Column::_spec_t;
				using _cpp_value_type = typename Column::_value_type::_cpp_value_type;

				column_t(const Column&)
				{}
				column_t(const column_t&) = default;
				column_t(column_t&&) = default;
				column_t& operator=(const column_t&) = default;
				column_t& operator=(column_t&&) = default;
				~column_t() = default;

				template<typename T>
				auto operator()(T& t) -> decltype(_column_spec_t::template accesssor_t<T>::get(t))&
				{
					return _column_spec_t::template accesssor_t<T>::get(t);
				}
			};
	}
	namespace vendor
	{
		template<typename... Args>
			struct interpreter_t<::sqlpp::container::context_t, column_t<Args...>>
			{
				using T = column_t<Args...>;

				static ::sqlpp::container::column_t<T> _(const T& t, ::sqlpp::container::context_t& context)
				{
					return { t };
				}
			};

	}
}

#endif
