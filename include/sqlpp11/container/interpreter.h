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
	namespace vendor
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

		template<typename Container, typename Database, typename Table, typename InsertList, typename ColumnList, typename ValueList>
			struct interpreter_t<::sqlpp::container::context_t<Container>, sqlpp::insert_t<Database, Table,  InsertList, ColumnList, ValueList>>
			{
				using Context = ::sqlpp::container::context_t<Container>;
				using T = sqlpp::insert_t<Database, Table, InsertList, ColumnList, ValueList>;
				using _assignment_tuple = typename InsertList::_parameter_tuple_t;

				template<typename Assignment>
				struct converter
				{
					using type = decltype(interpret(std::declval<T>(), std::declval<Context>()));
				};

				static auto _(const T& t, ::sqlpp::container::context_t<Container>& context)
					-> ::sqlpp::container::insert_t<decltype(tuple_interpreter<_assignment_tuple, ::sqlpp::container::context_t<Container>, 0, std::tuple_size<_assignment_tuple>::value>::_interpret(t._insert_list._assignments, context))>
				{
					 return { tuple_interpreter<_assignment_tuple, ::sqlpp::container::context_t<Container>, 0, std::tuple_size<_assignment_tuple>::value>::_interpret(t._insert_list._assignments, context) } ;
				}

			private:
			};

		/*
    template<typename Container,
      typename Database,
      typename FlagList,
      typename ColumnList,
      typename From,
      typename WhereCondition,
      typename GroupBy,
      typename Having,
      typename OrderBy,
      typename Limit,
      typename Offset
        >
        struct interpreter_t<::sqlpp::container::context_t<Container>, select_t<Database,
      FlagList,
      ColumnList,
      From,
      ::sqlpp::vendor::where_t<Database, WhereCondition>,
      GroupBy,
      Having,
      OrderBy,
      Limit,
      Offset>>
      {
        using T = select_t<Database,
        FlagList,
        ColumnList,
        From,
        ::sqlpp::vendor::where_t<Database, WhereCondition>,
        GroupBy,
        Having,
        OrderBy,
        Limit,
        Offset>;

				using condition_t = decltype(interpret(std::declval<WhereCondition>(), std::declval<::sqlpp::container::context_t<Container>>()));

				static ::sqlpp::container::select_t<Container, WhereCondition> _(const T& t, ::sqlpp::container::context_t<Container>& context)
				{
					return { t };
				}
			};
			*/

	}
}

#endif
