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

#ifndef SQLPP_CONTAINER_WRAP_OPERAND_H
#define SQLPP_CONTAINER_WRAP_OPERAND_H

namespace sqlpp
{
	namespace container
	{
		template<typename Operand>
			struct operand_t
			{
				using _cpp_value_type = typename Operand::_value_t;

				operand_t(const Operand& operand):
					_t(operand._t)
				{}
				operand_t(const operand_t&) = default;
				operand_t(operand_t&&) = default;
				operand_t& operator=(const operand_t&) = default;
				operand_t& operator=(operand_t&&) = default;
				~operand_t() = default;

				template<typename T>
				const _cpp_value_type operator()(const T&) const
				{
					return _t;
				}
				
				_cpp_value_type _t;
			};
	}
	namespace vendor
	{
		template<>
			struct interpreter_t<::sqlpp::container::context_t, boolean_operand>
			{
				using T = boolean_operand;

				static ::sqlpp::container::operand_t<T> _(const T& t, ::sqlpp::container::context_t& context)
				{
					return { t };
				}
			};

		template<>
			struct interpreter_t<::sqlpp::container::context_t, integral_operand>
			{
				using T = integral_operand;

				static ::sqlpp::container::operand_t<T> _(const T& t, ::sqlpp::container::context_t& context)
				{
					return { t };
				}
			};

		template<>
			struct interpreter_t<::sqlpp::container::context_t, floating_point_operand>
			{
				using T = floating_point_operand;

				static ::sqlpp::container::operand_t<T> _(const T& t, ::sqlpp::container::context_t& context)
				{
					return { t };
				}
			};

		template<>
			struct interpreter_t<::sqlpp::container::context_t, text_operand>
			{
				using T = text_operand;

				static ::sqlpp::container::operand_t<T> _(const T& t, ::sqlpp::container::context_t& context)
				{
					return { t };
				}
			};
	}
}

#endif
