module game;
import <format>; 
using gartic::Game;
using namespace gartic;
#include "StopWatch.h";

Game::Game()
{
}

Game::~Game()
{
}

void Game::SetPlayers(const std::vector<User> asd)
{
	this->m_Players = asd;
}

//void Game::SetPlayerScoreDefault(float score, User entity)
//{
//	m_Scores.insert({ score, entity.GetId() });
//}

void Game::SetScores(const std::map<std::string, std::vector<float>>& scor)
{
	m_Scores = scor;

}

void gartic::Game::addScore(std::string name, float score)
{
	for (auto i : m_Scores)
	{
		if (i.first == name)
		{
			i.second.push_back(score);
		}
	}
}

//void gartic::Game::addScore(const std::map<std::string, int>& scores)
//{
//	
//}

void Game::SetDrawer(const User& user)
{
	m_drawingPlayer = user;
}

const std::vector<User>& Game::GetPlayers() const
{
	return m_Players;
}

const std::unordered_set<std::string>& Game::GetUsedWords() const
{
	return m_usedWords;
}

std::vector<std::string>& Game::GetWords()
{
	return m_Words;
}

const std::vector<std::string>& Game::GetTips() const
{
	return m_Tips;
}

 std::map<std::string, std::vector<float>> Game::GetScores() 
{
	return m_Scores;
}

const User& Game::GetDrawer() const
{
	return m_drawingPlayer;
}

void Game::FileRead()
{
	std::ifstream file("GuessingWords.txt");
	std::string word;
	while (!file.eof())
	{
		file >> word;
		m_Words.push_back(word);
	}
	file.close();
	
}

const int& Game::GetCurrentRound(int Round)
{
	return Round;
}

void Game::GameOver(int currentRound = 0)
{
	while (currentRound < 4)	
	{ 
		if (currentRound == 4)
		{
			std::cout << "trebuie pus ceva cod aici ca sa compileze!";
		} // Animatie sfarsit de Joc / GAME OVER interface .
		else std::cout << this->GetCurrentRound(currentRound);
	}
}

double gartic::Game::CalculateScoreGuesser(double time)
{
	return (60 - time) * 100 / 30;
}

double gartic::Game::CalculateScorePainter(double averageTime)
{
	return (60 - averageTime) * 100 / 60;
}

//std::string gartic::Game::GetGuessedWord() const
//{
//	return m_guessedWord;
//}

Game& Game::operator=(const Game& other)
{
	if (this == &other)
	{
		return *this;
	}
	m_Players = other.m_Players;
	m_usedWords = other.m_usedWords;
	m_Words = other.m_Words;
	m_Scores = other.m_Scores;
	return *this;
}

Game::Game(Game&& other) noexcept
{
	m_Players = std::move(other.m_Players);
	m_usedWords = std::move(other.m_usedWords);
	m_Words = std::move(other.m_Words);
	m_Scores = std::move(other.m_Scores);
}

Game& Game::operator=(Game&& other) noexcept
{
	if (this == &other) {
		return *this;
	}
	m_Players = std::move(other.m_Players);
	m_usedWords = std::move(other.m_usedWords);
	m_Words = std::move(other.m_Words);
	m_Scores = std::move(other.m_Scores);
	return *this;

}

void Game::verifyGuessed()
{
	std::string introducedWord;
	std::cout << "Guess the word:";
	std::cin >> introducedWord;
	std::string wordToGuess = selectRandomWord(this->m_Words);
	AddUsedWord(wordToGuess);

	Stopwatch stopwatch;
	stopwatch.start();

	double elapsedTimeLimit = 60.0;
	while (introducedWord != wordToGuess )
	{
		std::cout<<wordToGuess;
		std::cin >> introducedWord;

	}

	stopwatch.stop();
	std::cout << stopwatch.elapsed_time()<<"\n";
	std::cout << "Congrats , you guessed the word";

}

std::string Game::selectRandomWord(std::vector<std::string>& m_Words)
{
	if (m_Words.empty()) {
		std::cerr << "m_Words vector is empty. Unable to select a random word." << std::endl;
		return "";
	}

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<size_t> distribution(0, m_Words.size() - 1);
	size_t randomIndex = distribution(gen);

	std::string selectedWord = m_Words[randomIndex];

	// Elimina elementul selectat din vector
	m_Words.erase(m_Words.begin() + randomIndex);

	return selectedWord;
}

void Game::AddUsedWord(const std::string& word) {
	m_usedWords.emplace(word);
}
