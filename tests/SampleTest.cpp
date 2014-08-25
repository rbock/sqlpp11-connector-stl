/*
 * Copyright (c) 2013, Roland Bock
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
#include <iostream>

#include "TabSample.h"
#include <sqlpp11/sqlpp11.h>
#include <sqlpp11/container/connection.h>

#include <vector>

namespace sql = sqlpp::container;
constexpr TabSample tab{};

struct sample
{
	int64_t alpha;
	std::string beta;
	bool gamma;
};

int main()
{
	using container = std::vector<sample>;
	container data;

	sql::connection<container> db{data};

	db(insert_into(tab).set(tab.alpha = 17));
	db(insert_into(tab).set(tab.beta = "cheesecake"));
	db(insert_into(tab).set(tab.alpha = 42, tab.beta = "hello", tab.gamma = true));
	db(insert_into(tab).set(tab.gamma = true));

	for (const sample& row: db(select(tab.alpha).from(tab).where(tab.alpha < 18 and tab.beta != "cheesecake").order_by(tab.alpha.desc())))
	{
		std::cerr << "alpha=" << row.alpha << ", beta=" << row.beta << ", gamma=" << row.gamma << std::endl;
	}

	return 0;
}
