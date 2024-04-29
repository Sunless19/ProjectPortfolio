#include "usersDB.h"

void populateStorage(Storage& storage)
{
	std::vector<gartic::User> users{
	};
	storage.sync_schema();
	storage.insert_range(users.begin(), users.end());
}

AddToUser::AddToUser(Storage& storage)
	:m_db{ storage }
{

}

crow::response AddToUser::operator()(const crow::request& req) const
{
	//extragerea datelor din cerere
	auto json = crow::json::load(req.body);
	
	
	if (!json)
	{
		return crow::response(400, "Invalid JSON in the request body");
	}
	//validarea si extragerea datelor utilizatorului
	gartic::User newUser;
	newUser.SetId(7);
	newUser.SetName(json["name"].s());

	auto newUserId = m_db.insert(newUser);
	
	return crow::response(201, "User added with ID: " + std::to_string(newUserId));
}


//std::vector<std::string> split(const std::string& str, const std::string& delim)
//{
//	std::vector<std::string> result;
//	size_t startIndex = 0;
//
//	for (size_t found = str.find(delim); found != std::string::npos; found = str.find(delim, startIndex))
//	{
//		result.emplace_back(str.begin() + startIndex, str.begin() + found);
//		startIndex = found + delim.size();
//	}
//	if (startIndex != str.size())
//		result.emplace_back(str.begin() + startIndex, str.end());
//	return result;
//}
//
//std::unordered_map<std::string, std::string> parseUrlArgs(const std::string& urlArgs)
//{
//	if (urlArgs == "") {
//		return {};
//	}
//
//	std::unordered_map<std::string, std::string> result;
//	for (const auto& kvStr : split(urlArgs, "&")) {
//		auto kvVector = split(kvStr, "=");
//		if (kvVector.size() == 2)
//			result[kvVector[0]] = kvVector[1];
//	}
//	return result;
//}
