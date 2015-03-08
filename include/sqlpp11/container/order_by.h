
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

#ifndef SQLPP_CONTAINER_ORDER_BY_H
#define SQLPP_CONTAINER_ORDER_BY_H

#include <sqlpp11/detail/index_sequence.h>

namespace sqlpp
{
	namespace container
	{
		template<typename T>
			struct order_by_data_t
			{
				static_assert(::sqlpp::wrong_t<order_by_data_t>::value, "invalid argument for order_by");
			};

		template<typename... Expressions>
			struct order_by_data_t<std::tuple<Expressions...>>
			{
				template<size_t> struct index {};

				template<typename T>
					bool operator()(const T& lhs, const T& rhs) const
					{
						return eval(lhs.get(), rhs.get(), index<0>{});
					}

				template<typename T, size_t i>
					bool eval(const T& lhs, const T& rhs, const index<i>&) const
					{
						return std::get<i>(_expressions)(lhs, rhs) and eval(lhs, rhs, index<i+1>{});
					}

				template<typename T>
					bool eval(const T& lhs, const T& rhs, const index<sizeof...(Expressions)>&) const
					{
						return true;
					}

				std::tuple<Expressions...> _expressions;
			};
	}
}

#endif
