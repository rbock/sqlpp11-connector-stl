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
#include <sqlpp11/container/select.h>

namespace sqlpp
{
	template<typename Tuple, typename Context, size_t i, size_t N>
			struct tuple_interpreter
			{
				static auto _interpret(const Tuple& tup, Context& context)
					-> decltype(std::tuple_cat(std::make_tuple(interpret(std::get<i>(tup), context)), tuple_interpreter<Tuple, Context, i + 1, N>::_interpret(tup, context)))
					{
						return  std::tuple_cat(std::make_tuple(interpret(std::get<i>(tup), context)), tuple_interpreter<Tuple, Context, i + 1, N>::_interpret(tup, context));
					}
			};

		template<typename Tuple, typename Context, size_t N>
			struct tuple_interpreter<Tuple, Context, N, N>
			{
				static auto _interpret(const Tuple&, Context&)
					-> std::tuple<>
					{
						return {};
					}
			};

		template<typename Database, typename Table, typename InsertValueList>
			struct interpreter_t<::sqlpp::container::context_t, sqlpp::statement_t<Database, insert_t, Table,  InsertValueList>>
			{
				using Context = ::sqlpp::container::context_t;
				using T = sqlpp::statement_t<Database, insert_t, Table, InsertValueList>;
				using _assignment_tuple = typename InsertValueList::_data_t::_assignment_tuple;

				static auto _(const T& t, ::sqlpp::container::context_t& context)
					-> ::sqlpp::container::insert_t<decltype(tuple_interpreter<_assignment_tuple, ::sqlpp::container::context_t, 0, std::tuple_size<_assignment_tuple>::value>::_interpret(t.insert_list._data._assignments, context))>
				{
					 return { tuple_interpreter<_assignment_tuple, ::sqlpp::container::context_t, 0, std::tuple_size<_assignment_tuple>::value>::_interpret(t.insert_list._data._assignments, context) } ;
				}

			private:
			};

}

#endif
