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


#ifndef SQLPP_CONTAINER_CONNECTION_H
#define SQLPP_CONTAINER_CONNECTION_H

#include <string>
#include <algorithm>
#include <sqlpp11/connection.h>

namespace sqlpp
{
	namespace container
	{
		namespace detail
		{
			template<typename Connection, typename Argument>
			 struct	command_runner
			 {
				 static auto run(Connection& db, const Argument& t) -> decltype(t._run(db))
				 {
					 return t._run(db);
				 }
			 };

			template<typename Connection,
				typename Database,
				typename With,
				typename... Args
					>
					struct command_runner<Connection, statement_t<Database, With, select_t, Args...>>
				{
					using Select = statement_t<Database,With, select_t, Args...>;

					static auto run(Connection& db, const Select& select) -> decltype(db.select(select))
					{
						return db.select(select);
					}
				};
		}

		struct context_t
		{
			using is_container_context = std::true_type;

			context_t()
			{}
		};

		template<typename Container>
			class connection: public sqlpp::connection
		{
			Container _data;
			using _row_t = typename Container::value_type;
			using _result_row_t = std::reference_wrapper<_row_t>;

		public:
			using _context_t = context_t;
			using _result_t = std::vector<_result_row_t>;

			connection(Container data): _data(data) {}
			connection(const connection&) = delete;
			connection(connection&&) = delete;
			connection& operator=(const connection&) = delete;
			connection& operator=(connection&&) = delete;
			~connection() = default;

			const Container& get_container() const
			{
				return _data;
			}

			template<typename Select>
				auto select(Select& s)
				-> _result_t
				{
					_context_t context;
					auto condition = interpret(std::get<0>(s.where._data._expressions), context);

					_result_t result;

					std::copy_if(_data.begin(), _data.end(), std::back_inserter(result), condition);

					auto less = interpret(s.order_by._data, context);
					std::sort(result.begin(), result.end(), less);

					return result;
				}


			//! insert returns the last auto_incremented id (or zero, if there is none)
			template<typename Insert>
				size_t insert(const Insert& i)
				{
					_context_t context;
					auto assign = interpret(i, context);
					_row_t v{};
					assign(v);
					_data.push_back(v);
					return 0;
				}

			//! call run on the argument
			template<typename T>
				auto operator()(const T& t) -> decltype(detail::command_runner<connection, T>::run(*this, t))
				{
					return detail::command_runner<connection, T>::run(*this, t);
				}

		};

	}
}

#include <sqlpp11/container/interpreter.h>

#endif
