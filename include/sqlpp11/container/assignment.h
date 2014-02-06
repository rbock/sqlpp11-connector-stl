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

#ifndef SQLPP_CONTAINER_ASSIGNMENT_H
#define SQLPP_CONTAINER_ASSIGNMENT_H

namespace sqlpp
{
	namespace container
	{
		template<typename Lhs, typename Rhs>
			struct assignment_t
			{
				assignment_t(Lhs l, Rhs r):
					_lhs(l),
					_rhs(r)
				{}
				assignment_t(const assignment_t&) = default;
				assignment_t(assignment_t&&) = default;
				assignment_t& operator=(const assignment_t&) = default;
				assignment_t& operator=(assignment_t&&) = default;
				~assignment_t() = default;

				template<typename T>
				void operator()(T& t)
				{
					_lhs(t) = _rhs(t);
				}

				Lhs _lhs;
				Rhs _rhs;
			};
	}

	namespace vendor
	{
		template<typename Container, typename Lhs, typename Rhs>
			struct interpreter_t<::sqlpp::container::context_t<Container>, assignment_t<Lhs, Rhs>>
			{
				using T = assignment_t<Lhs, Rhs>;

				static auto _(const T& t, ::sqlpp::container::context_t<Container>& context)
					-> ::sqlpp::container::assignment_t<decltype(interpret(t._lhs, context)), decltype(interpret(t._rhs, context))>
				{
					return { interpret(t._lhs, context), interpret(t._rhs, context) };
				}
			};

	}
}

#endif
