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

#ifndef SQLPP_CONTAINER_EXPRESSION_H
#define SQLPP_CONTAINER_EXPRESSION_H

#include <functional>

namespace sqlpp
{
	namespace container
	{
		namespace detail
		{
			template<typename Operator>
				struct operator_map
				{
					static_assert(::sqlpp::wrong_t<Operator>::value, "this operator is not implemented yet");
				};

			template<>
				struct operator_map<sqlpp::op::less>
				{
					template<typename T>
						using operator_t = std::less<T>;
				};


			template<>
				struct operator_map<sqlpp::op::less_equal>
				{
					template<typename T>
						using operator_t = std::less_equal<T>;
				};

			template<>
				struct operator_map<sqlpp::op::equal_to>
				{
					template<typename T>
						using operator_t = std::equal_to<T>;
				};

			template<>
				struct operator_map<sqlpp::op::not_equal_to>
				{
					template<typename T>
						using operator_t = std::not_equal_to<T>;
				};

			template<>
				struct operator_map<sqlpp::op::greater_equal>
				{
					template<typename T>
						using operator_t = std::greater_equal<T>;
				};

			template<>
				struct operator_map<sqlpp::op::greater>
				{
					template<typename T>
						using operator_t = std::greater<T>;
				};

			template<>
				struct operator_map<sqlpp::op::logical_and>
				{
					template<typename T>
						using operator_t = std::logical_and<T>;
				};

			template<>
				struct operator_map<sqlpp::op::logical_or>
				{
					template<typename T>
						using operator_t = std::logical_or<T>;
				};

			template<typename ValueType>
				struct operator_map<sqlpp::op::plus<ValueType>>
				{
					template<typename T>
						using operator_t = std::plus<T>;
				};

			template<typename ValueType>
				struct operator_map<sqlpp::op::minus<ValueType>>
				{
					template<typename T>
						using operator_t = std::minus<T>;
				};

			template<typename ValueType>
				struct operator_map<sqlpp::op::multiplies<ValueType>>
				{
					template<typename T>
						using operator_t = std::multiplies<T>;
				};

			template<>
				struct operator_map<sqlpp::op::divides>
				{
					template<typename T>
						using operator_t = std::divides<typename sqlpp::value_type_of<sqlpp::op::divides>::_cpp_value_type>;
				};

			template<>
				struct operator_map<sqlpp::op::modulus>
				{
					template<typename T>
						using operator_t = std::modulus<typename sqlpp::value_type_of<sqlpp::op::modulus>::_cpp_value_type>;
				};

		};

		template<typename Lhs, typename Operator, typename Rhs>
			struct binary_expression_t
			{
				using operator_t = typename detail::operator_map<Operator>::template operator_t<typename Lhs::_cpp_value_type>;
				using _cpp_value_type = typename value_type_of<Operator>::_cpp_value_type;

				binary_expression_t(Lhs l, Rhs r):
					_lhs(l),
					_rhs(r)
				{}
				binary_expression_t(const binary_expression_t&) = default;
				binary_expression_t(binary_expression_t&&) = default;
				binary_expression_t& operator=(const binary_expression_t&) = default;
				binary_expression_t& operator=(binary_expression_t&&) = default;
				~binary_expression_t() = default;

				template<typename T>
				auto operator()(T& t)
				-> _cpp_value_type
				{
					return operator_t()(_lhs(t), _rhs(t));
				}

				Lhs _lhs;
				Rhs _rhs;
			};
	}

	template<typename Lhs, typename O, typename Rhs>
		struct interpreter_t<::sqlpp::container::context_t, binary_expression_t<Lhs, O, Rhs>>
		{
			using T = binary_expression_t<Lhs, O, Rhs>;

			static auto _(const T& t, ::sqlpp::container::context_t& context)
				-> ::sqlpp::container::binary_expression_t<decltype(interpret(t._lhs, context)), O, decltype(interpret(t._rhs, context))>
				{
					return { interpret(t._lhs, context), interpret(t._rhs, context) };
				}
		};

	template<typename Operand>
		struct interpreter_t<::sqlpp::container::context_t, tvin_t<Operand>>
		{
			using T = tvin_t<Operand>;

			static auto _(const T& t, ::sqlpp::container::context_t& context)
				-> decltype(interpret(t._value, context))
				{
					// FIXME: need to assert, that Operand is not tvin!
					return interpret(t._value, context);
				}
		};
}

#endif
