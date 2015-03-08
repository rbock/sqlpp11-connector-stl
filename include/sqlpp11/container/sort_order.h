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

#ifndef SQLPP_CONTAINER_SORT_ORDER_H
#define SQLPP_CONTAINER_SORT_ORDER_H

#include <sqlpp11/detail/index_sequence.h>

namespace sqlpp
{
	namespace container
	{
		template<typename Expression, sort_type SortType>
			struct sort_order_t
			{
				template<typename T>
				bool operator()(const T& lhs, const T& rhs) const
				{
					if (SortType == sort_type::asc)
						return _expression.less(lhs, rhs);
					else
						return _expression.greater(lhs, rhs);
				}

				Expression _expression;

			};
	}

	template<typename Expression, sort_type SortType>
		struct interpreter_t<::sqlpp::container::context_t, sort_order_t<Expression, SortType>>
		{
			using T = sort_order_t<Expression, SortType>;

			static auto _(const T& t, ::sqlpp::container::context_t& context)
				-> container::sort_order_t<decltype(interpret(t._expression, context)), SortType>
			{
				return { interpret(t._expression, context) };
			}
		};
}

#endif
