module user;
import <format>;

using gartic::User;


User::User()
{
}

//User::User(const uint32_t& id, const std::string& name, const std::vector<std::pair<uint32_t, int>>& matchHistory, const float historyAverage)
//	: m_id(id)
//	, m_name(name)
//	, m_matchHistory(matchHistory)
//	, m_historyAverage(historyAverage)
//
//{
//}

//User::User(const uint32_t& id, const std::string& name, const float historyAverage)
//	: m_id(id), 
//	m_name(name), 
//	m_historyAverage(historyAverage)
//{
//}


User::User(const uint32_t& id = -1, const std::string& name="") :
	m_id(id),
	m_name(name)
{
}

int User::GetId() const
{
	return m_id;
}

std::string User::GetName() const
{
	return m_name;
}

//std::vector<std::pair<uint32_t, int>> User::getMatchHistory() const
//{
//	return m_matchHistory;
//}

//float User::getHistoryAverage() const
//{
//	return m_historyAverage;
//}

bool User::GetGuessed() const
{
	return this->m_guessed;
}

bool User::GetDrawing() const
{
	return this->m_isDrawing;
}

void User::SetGuessed()
{
	this->m_guessed = true;
}

void User::SetDrawingState(bool state)
{
	this->m_isDrawing = state;
}

void User::SetId(int id)
{
	m_id = id;
}

void User::SetName(const std::string& name)
{
	m_name = name;
}

float gartic::User::GetScore() const
{
	return this->score;
}

void gartic::User::SetScore(float newScore)
{
	this->score = newScore;
}

//void User::setMatchHistory(const std::vector<std::pair<uint32_t, int>>& matchHistory)
//{
//	m_matchHistory = matchHistory;
//}

//void gartic::User::setHistoryAverage(const float& historyAverage)
//{
//	if (historyAverage < 0.0f) {
//		calculateHistoryAverage();
//	}
//	else {
//		m_historyAverage = historyAverage;
//	}
//	m_historyAverage = historyAverage;
//}

//void gartic::User::calculateHistoryAverage()
//{
//		int size = int(this->getMatchHistory().size());
//	float sum = 0;
//	for (std::pair<uint32_t, int> match : getMatchHistory())
//		sum += match.second;
//	m_historyAverage = sum / size;
//}



//void User::setHistoryAverage(const std::vector<std::pair<uint32_t, int>>& matchHistory)
//{
//	int size = int(this->getMatchHistory().size());
//	float sum = 0;
//	for (std::pair<uint32_t, int> match : getMatchHistory())
//		sum += match.second;
//	m_historyAverage = sum / size;
//}

bool User::ValidateName(const std::string& name)
{
	std::regex pattern("^(?=.*[a-zA-Z])(?=.*[0-9]).+$");
	if (std::regex_match(name, pattern))
	{
		return true;
	}
	else
	{
		std::cout << "The Name " << name << " is not valid!";
		return false;
	}
	return true;  // Adaug? aceast? linie pentru a trata cazul în care numele este valid.
}

User& User::operator=(const User& other)
{
	if (this == &other)
	{
		return*this;
	}
	m_id = other.m_id;
	m_name = other.m_name;
	/*m_matchHistory = other.m_matchHistory;
	m_historyAverage = other.m_historyAverage;*/
	return *this;
}

User::User(User&& other) noexcept
{
	m_id = other.m_id;
	m_name = std::move(other.m_name);
	/*m_matchHistory = std::move(other.m_matchHistory);
	m_historyAverage = other.m_historyAverage;*/
}

User::User(const User& other) noexcept
{
	*this = other;
}

User& User::operator=(User&& other) noexcept
{
	if (this == &other)
	{
		return *this;
	}
	m_id = other.m_id;
	m_name = std::move(other.m_name);
	return *this;
}

std::ostream& gartic::operator<<(std::ostream& os, const User& user)
{
	os << "ID:" << user.GetId() << "\nName:" << user.GetName() << "\n";
	return os;
}