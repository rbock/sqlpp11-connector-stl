sqlpp11-connector-stl
=====================

An experimental SQL connector for containers and streams of the C++ Standard Library

Motivation:
-----------
Well, sqlpp11 has this wonderful expression tree. It makes it really easy to connect to databases. And you know what? You can even treat containers or streams from the C++ Standard Library as databases! How cool is that?

```C++
namespace sql = sqlpp::container;

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

	sql::connection<container> db(data);
	sample tab;

	db.run(insert_into(tab).set(tab.alpha = 17));
	db.run(insert_into(tab).set(tab.beta = "cheesecake"));
	db.run(insert_into(tab).set(tab.alpha = 42, tab.beta = "hello", tab.gamma = true));
	db.run(insert_into(tab).set(tab.gamma = true));

	for (const sample& row: db.run(select(all_of(tab(tab))
	                                 .from(tab)
	                                 .where(tab.alpha < 18)))
	{
		std::cerr << "alpha=" << row.alpha << 
		           ", beta=" << row.beta << 
		           ", gamma=" << row.gamma << std::endl;
	}

	return 0;
}
```

That works???

Oh yes! Check it out. You need:

 * A current C++11 compiler (clang-3.2 is known to work on Ubuntu, g++4.8 is missing std::copy_if)
 * sqlpp11: https://github.com/rbock/sqlpp11
 * And this library, of course
 
Stay tuned for more :-)

