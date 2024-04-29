#include <ctime>
#define EXCLUDE_WINDOWS_TIME_H
#include <sys/stat.h>
#define EXCLUDE_WINDOWS_STAT_H
#include "StopWatch.h"
import user;
import game;
import<iostream>;
import<vector>;
#include<deque>
#include<filesystem>
#include<memory>
#include <crow.h>
#include "usersDB.h"
#include <algorithm>
#include <crow/websocket.h>
#include <sstream>
#include <cpr/cpr.h>
using namespace gartic;

void run(const std::vector<crow::json::wvalue>& gameVerify, const std::vector<crow::json::wvalue>& active, Game& gameInstance, int& iterator)
{
	//alegem cuvant si drawer.
	if (gameVerify.size() > 0)
	{
		auto usersResponse = cpr::Get(cpr::Url{ "http://localhost:18080/getUsers" });
		auto activeUsers = crow::json::load(usersResponse.text);
		std::cout << usersResponse.text;

		auto usersResponse1 = cpr::Get(cpr::Url{ "http://localhost:18080/users" });
		auto Users = crow::json::load(usersResponse1.text);
		std::cout << usersResponse1.text;


		//folosesc smart pointer de tip:shared_ptr:acesta faciliteaza ciclul de viata a obiecetlor de tip User activi
		// si evita scurgerile de memorie at cand obiectele nu mai sunt accesate.Contine Rule of 5 acre faciliteaza eficienta 
		std::vector<std::shared_ptr<User>> actives;//

		for (auto userAct : activeUsers)
		{
			for (auto user : Users)
			{
				if (userAct["name"].s() == user["name"].s())
				{
					auto userPtr = std::make_shared<User>(user["id"].i(), userAct["name"].s());
					actives.push_back(userPtr);
				}
			}
		}
		//gameInstance.SetPlayers(actives);
		// Extrage obiectele User din obiectele shared_ptr,le adauga intr-un vector separat de  gartic::User pt a putea apea setPlayers
		std::vector<gartic::User> users;                                    //
		for (const auto& userPtr : actives) {
			users.push_back(*userPtr);
		}
		gameInstance.SetPlayers(users);


		for (auto x : gameInstance.GetPlayers())
			std::cout << x.GetName() << " ";

		//Send word to server->client.
		gameInstance.FileRead();
		//std::string word = gameInstance.selectRandomWord(gameInstance.GetWords());




		bool userType;
		while (iterator / actives.size() != 4)
		{
			//vectorul de activi e de tip pointer
			gameInstance.SetDrawer(*actives[iterator % actives.size()]);//
			//gameInstance.SetDrawer(actives[iterator % actives.size()]);
			std::cout << gameInstance.GetDrawer().GetName() << "\n\n\n\n";

			std::string word = gameInstance.selectRandomWord(gameInstance.GetWords());

			cpr::Parameters parameters = {
				{"word", word}, };

			auto response = cpr::Put(cpr::Url{ "http://localhost:18080/randomWord" }, parameters);
			std::cout << response.text;
			if (response.status_code == 200)
				std::cout << "Cuvântul a fost trimis cu succes la server!\n";
			else std::cerr << "Eroare la trimiterea cuvântului la server.\n";

			for (auto y : actives)
			{
				cpr::Response response = cpr::Get(cpr::Url{ "http://localhost:18080/get_random_word" });
				auto jsonword = crow::json::load(response.text);

				if (y->GetName() == gameInstance.GetDrawer().GetName())
				{


					cpr::Parameters parameters = {
							{"name", y->GetName()},
							{"guesser","true"},
							{"word",jsonword["word"].s()} };

					auto response = cpr::Put(cpr::Url{ "http://localhost:18080/UserType" }, parameters);

				}
				if (y->GetName() != gameInstance.GetDrawer().GetName())
				{
					cpr::Parameters parameters = {
							{"name", y->GetName()},
							{"guesser","false"},
							{"word",jsonword["word"].s()} };
					auto response = cpr::Put(cpr::Url{ "http://localhost:18080/UserType" }, parameters);
				}
			}
			iterator++;
		}
		/*
			for (auto y : actives)
			{
				cpr::Parameters parameters = {
										{"name", y.GetName()},
										{"guesser","end"}, };
				auto response = cpr::Put(cpr::Url{ "http://localhost:18080/UserType" }, parameters);
			}*/

	}
}

int main()
{
	/*-----------------------------------------Instantiation-----------------------------------------------*/

	//using namespace gartic;
	const std::string db_file = "products.sqlite";
	Storage db = createStorage(db_file);
	std::vector<crow::json::wvalue> chatMessages;
	std::vector<crow::json::wvalue> gameVerify;
	std::vector<crow::json::wvalue> active;
	std::vector<crow::json::wvalue> interfaces;
	std::vector<crow::json::wvalue> userType;
	crow::json::wvalue randomWord;
	crow::json::wvalue lobbyActive;
	std::vector<std::pair<std::string, bool>> nameBoolPairs; // Vector pentru a stoca perechi de nume și valori bool

	int iterator = 0;

	crow::SimpleApp app;

	//unique_ptr:ajuta la gestionarea automata a memoriei si prevenirea scurgerilor de memorie,se foloseste unique_ptr pt ca jocul e unic. 
	std::unique_ptr<Game> gameInstance = std::make_unique<Game>();//Smart pointer de tip unique_ptr pentru joc
	//Game gameInstance;
	db.sync_schema();

	auto initUsersCount = db.count<User>();
	if (initUsersCount == 0)
		populateStorage(db);

	auto usersCount = db.count<User>();


	/*-----------------------------------------ROUTING-----------------------------------------------*/

	CROW_ROUTE(app, "/")([]() {
		return "<html>"
			"<body>"
			"<p>This is the main lobby</p>"
			"<p>Go to the Game section: <a href=\"/game\">Game</a></p>"
			"<p>Go to the Users section: <a href=\"/users\">users</a></p>"
			"<p>Go to the chat section: <a href=\"/chat\">chat</a></p>"
			"<p>Go to the Drawer's section: <a href=\"/drawer\">WordToDraw</a></p>"
			"<p>Go to the Guesser's section: <a href=\"/guesser\">WordToGuess</a></p>"
			"</body>"
			"</html>";
		});

	CROW_ROUTE(app, "/game")([]() {
		return "This is the Game section";
		});

	CROW_ROUTE(app, "/startGame").methods(crow::HTTPMethod::Put)([&gameVerify, &active, &gameInstance, &iterator](const crow::request& req) {

		iterator;
		std::string start{ req.url_params.get("start") };
		gameVerify.push_back(crow::json::wvalue{ { "start",start } });
		run(gameVerify, active, *gameInstance, iterator);
		return crow::response(200);
		});

	CROW_ROUTE(app, "/checkStarted").methods(crow::HTTPMethod::Get)([&gameVerify](const crow::request& req) {
		// Verificați starea butonului aici, folosind informațiile stocate în gameVerify
		// De exemplu, puteți verifica dacă există o intrare pentru "start" și dacă are o anumită valoare

		std::vector<crow::json::wvalue> jsonMessage;
		for (auto x : gameVerify)
		{
			jsonMessage.push_back(crow::json::wvalue{ {x} });
		}
		return crow::json::wvalue{ jsonMessage };


		});

	CROW_ROUTE(app, "/randomWord").methods(crow::HTTPMethod::Put)([&randomWord](const crow::request& req) {

		std::string word{ req.url_params.get("word") };

		randomWord = crow::json::wvalue{ {"word",word} };

		return crow::response(200);
		});

	CROW_ROUTE(app, "/activeUsers").methods(crow::HTTPMethod::Put)([&active](const crow::request& req) {

		std::string name{ req.url_params.get("name") };

		active.push_back(crow::json::wvalue{
			{"name", name}
			});


		return crow::response(200);
		});

	CROW_ROUTE(app, "/lobbyActive").methods(crow::HTTPMethod::Put)([&lobbyActive](const crow::request& req)
		{
			std::string name{ req.url_params.get("status") };
			std::string namex{ req.url_params.get("name") };

			lobbyActive = crow::json::wvalue{ {"status",name},{"name",namex} };

			return crow::response(200);
		});

	CROW_ROUTE(app, "/uploadImage").methods(crow::HTTPMethod::Post)([&](const crow::request& req) {
		// Handle the image upload here

		// Get the uploaded image data from the request body
		const auto& body = req.body;
		std::string imageData(body.data(), body.size());

		// Save the image data to a file (you may need to adjust the path)
		std::ofstream imageFile("uploaded_image.png", std::ios::binary);
		imageFile.write(imageData.c_str(), imageData.size());

		return crow::response(200, "Image uploaded successfully");
		});

	CROW_ROUTE(app, "/downloadImage").methods(crow::HTTPMethod::Get)([&](const crow::request& req) {
		// Handle the image download here

		// Load the image from file (you may need to adjust the path)
		std::ifstream imageFile("uploaded_image.png", std::ios::binary);
		std::ostringstream imageData;
		imageData << imageFile.rdbuf();

		// Respond with the image data
		return crow::response(200, imageData.str());
		});

	CROW_ROUTE(app, "/getLobbyActive").methods("GET"_method)([&lobbyActive]() {
		return lobbyActive;
		});

	CROW_ROUTE(app, "/verifyMethod").methods("GET"_method)([&gameVerify, &active, &gameInstance, &iterator]() {

		run(gameVerify, active, *gameInstance, iterator);
		return crow::response(200);
		});

	CROW_ROUTE(app, "/users")([&db]()
		{
			std::vector<crow::json::wvalue> users_json;

			for (const auto& user : db.iterate<User>())
			{
				users_json.push_back(crow::json::wvalue{
					{"id", user.GetId()},
					{"name", user.GetName()},
					{"score",user.GetScore()}
					});
			}
			return crow::json::wvalue{ users_json };
		});

	CROW_ROUTE(app, "/adduser").methods(crow::HTTPMethod::Put)([&db](const crow::request& req)
		{
			std::string name{ req.url_params.get("name") };

			gartic::User newUser;
			newUser.SetName(name);
			newUser.SetScore(0.0);
			db.insert(newUser);

			return crow::response(200);
		});
	CROW_ROUTE(app, "/putscore").methods(crow::HTTPMethod::Put)([&gameInstance](const crow::request& req)
		{
			std::string name{ req.url_params.get("name") };
			std::string score {req.url_params.get("score") };

			gameInstance->addScore(name, std::stof(score));
			return crow::response(200);
		});

	CROW_ROUTE(app, "/get_score").methods("GET"_method)([&gameInstance]() {
		crow::json::wvalue jsonScore;

		for (auto x : gameInstance->GetScores())
		{
			jsonScore[x.first]=x.second;
		}
		return crow::json::wvalue{ jsonScore };
		});

	CROW_ROUTE(app, "/get_chat").methods("GET"_method)([&chatMessages]() {
		std::vector<crow::json::wvalue> jsonMessages;
		for (auto x : chatMessages)
		{
			jsonMessages.push_back(crow::json::wvalue{ {x} });
		}
		return crow::json::wvalue{ jsonMessages };
		});

	CROW_ROUTE(app, "/chat").methods(crow::HTTPMethod::Put)([&chatMessages](const crow::request& req) {
		// Extract the message from the request body
		std::string message{ req.url_params.get("Message") };
		std::string name{ req.url_params.get("Username") };

		// Store the message in the chatMessages deque
		chatMessages.push_back(crow::json::wvalue{
			{"Message",message},
			{"Username",name}
			});

		return crow::response(200);
		});
	CROW_ROUTE(app, "/getUsers").methods("GET"_method)([&active]() {
		std::vector<crow::json::wvalue> jsonMessages;
		for (auto x : active)
		{
			jsonMessages.push_back(crow::json::wvalue{ {x} });
		}
		return crow::json::wvalue{ jsonMessages };
		});

	CROW_ROUTE(app, "/get_random_word").methods("GET"_method)([&randomWord]()
		{
			return randomWord;
		});

	CROW_ROUTE(app, "/UserType").methods(crow::HTTPMethod::Put)([&userType](const crow::request& req) {
		// Extract the message from the request body
		std::string name{ req.url_params.get("name") };
		std::string type{ req.url_params.get("guesser") };
		std::string word{ req.url_params.get("word") };

		// Store the message in the chatMessages deque
		userType.push_back(crow::json::wvalue{
			{"name",name},
			{"guesser",type},
			{"word",word} });
		return crow::response(200);
		});

	CROW_ROUTE(app, "/getUserType").methods("GET"_method)([&userType]() {
		std::vector<crow::json::wvalue> jsonTypes;
		for (auto x : userType)
		{
			jsonTypes.push_back(crow::json::wvalue{ {x} });
		}
		return crow::json::wvalue{ jsonTypes };
		});

	app.port(18080).multithreaded().run();

	return 0;
}


//CROW_ROUTE(app, "/get_interface_type").methods("GET"_method)([&nameBoolPairs, &activeConnections](const crow::request& req) {
//	auto pairIter = nameBoolPairs.begin(); // Iterator pentru vectorul de perechi nume-bool
//	std::vector<crow::json::wvalue> interfaceTypes;
//	for (auto& conn : activeConnections) {
//		// Aici poți crea un obiect JSON pentru fiecare pereche nume-bool
//		crow::json::wvalue message;
//		message["name"] = pairIter->first;
//		message["boolValue"] = pairIter->second;
//		interfaceTypes.push_back(message);
//		++pairIter; // Mergi la următoarea pereche din vector
//	}
//	return crow::json::wvalue{ interfaceTypes };
//	});



