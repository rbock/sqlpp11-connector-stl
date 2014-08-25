/*
 * Copyright (c) 2013, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_CONTAINER_INTERPRETER_H
#define SQLPP_CONTAINER_INTERPRETER_H

#include <sqlpp11/container/column.h>
#include <sqlpp11/container/wrap_operand.h>
#include <sqlpp11/container/assignment.h>
#include <sqlpp11/container/expression.h>
#include <sqlpp11/container/insert.h>
#include <sqlpp11/container/order_by.h>
#include <sqlpp11/container/sort_order.h>
#include <sqlpp11/container/select.h>

namespace sqlpp
{
	namespace container
	{
		template<typename... T, size_t... Idx, typename Context>
			auto interpret_tuple(const std::tuple<T...>& tup, const sqlpp::detail::index_sequence<Idx...>&, Context& context)
			-> std::tuple<decltype(interpret(std::get<Idx>(tup), context))...>
			{
				return {interpret(std::get<Idx>(tup), context)...};
			}
	}

	template<typename Database, typename Table, typename InsertValueList>
		struct interpreter_t<::sqlpp::container::context_t, sqlpp::statement_t<Database, insert_t, Table,  InsertValueList>>
		{
			using Context = ::sqlpp::container::context_t;
			using T = sqlpp::statement_t<Database, insert_t, Table, InsertValueList>;
			using _assignment_tuple = decltype(InsertValueList::_data_t::_assignments);
			using _assignment_tuple_idx = sqlpp::detail::make_index_sequence<std::tuple_size<_assignment_tuple>::value>;

			static auto _(const T& t, ::sqlpp::container::context_t& context)
				-> ::sqlpp::container::insert_t<decltype(container::interpret_tuple(t.insert_list._data._assignments, _assignment_tuple_idx(), context)), 
				                                _assignment_tuple_idx>
				{
					return { container::interpret_tuple(t.insert_list._data._assignments, _assignment_tuple_idx(), context) } ;
				}

		private:
		};

	template<typename Database, typename... Expressions>
		struct interpreter_t<::sqlpp::container::context_t, sqlpp::order_by_data_t<Database, Expressions...>>
		{
			using Context = ::sqlpp::container::context_t;
			using T = sqlpp::order_by_data_t<Database, Expressions...>;
			using _expression_tuple = decltype(T::_expressions);
			using _expression_tuple_idx = sqlpp::detail::make_index_sequence<std::tuple_size<_expression_tuple>::value>;

			static auto _(const T& t, ::sqlpp::container::context_t& context)
				-> ::sqlpp::container::order_by_data_t<decltype(container::interpret_tuple(t._expressions, _expression_tuple_idx(), context))>
				{
					return { container::interpret_tuple(t._expressions, _expression_tuple_idx(), context) } ;
				}
		private:
		};

	template<typename Expr, bool TrivialValueIsNull>
		struct interpreter_t<::sqlpp::container::context_t, rhs_wrap_t<Expr, TrivialValueIsNull>>
		{
			using T = rhs_wrap_t<Expr, TrivialValueIsNull>;

			static auto _(const T& t, ::sqlpp::container::context_t& context)
				-> decltype(interpret(t._expr, context))
				{
					return interpret(t._expr, context);
				}
		};


}

#endif
