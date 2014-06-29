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

#ifndef SQLPP_CONTAINER_INSERT_H
#define SQLPP_CONTAINER_INSERT_H

namespace sqlpp
{
	namespace container
	{
		template<typename T>
			struct insert_t
			{
				static_assert(::sqlpp::wrong_t<T>::value, "invalid argument for index_t");
			};

		template<typename... Assignments>
			struct insert_t<std::tuple<Assignments...>>
			{
				template<size_t> struct index {};

				template<typename T>
					void operator()(T& lhs)
					{
						_assign(lhs, index<0>());
					}

				template<typename T, size_t i>
					void _assign(T& lhs, const index<i>&)
					{
						std::get<i>(_assignments)(lhs);
						_assign(lhs, index<i+1>());
					}

				template<typename T>
					void _assign(T& lhs, const index<sizeof...(Assignments)>&)
					{
					}


				std::tuple<Assignments...> _assignments;
			};
	}
}

#endif
