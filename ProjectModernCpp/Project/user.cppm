export module user;
import<iostream>;
import <string>;
import <vector>;
import <regex>;
namespace gartic
{
	export class User
	{
	public:
		User();
		/*User(const uint32_t& id, const std::string& name, const std::vector<std::pair<uint32_t, int>>& matchHistory, const float historyAverage);
		User(const uint32_t& id, const std::string& name, const float historyAverage);*/
		User(const uint32_t& id, const std::string& name);

		int GetId() const;
		std::string GetName() const;
		/*std::vector<std::pair<uint32_t, int>> getMatchHistory() const;
		float getHistoryAverage() const; */
		bool GetGuessed() const;
		bool GetDrawing() const;

		void SetGuessed();
		void SetDrawingState(bool state);
		void SetId(int id);
		void SetName(const std::string& name);
		float GetScore() const;
		void SetScore(float newScore);
		/*void setMatchHistory(const std::vector<std::pair<uint32_t, int>>& matchHistory);
		void calculateHistoryAverage();
		void setHistoryAverage(const float& historyAverage);*/
		//void setHistoryAverage(const std::vector<std::pair<uint32_t, int>>& matchHistory);

		bool ValidateName(const std::string& name);

		User& operator=(const User& other);//copy assigmnent
		User(User&& other) noexcept;//move constructor
		User(const User& other) noexcept;//copy constructor
		User& operator=(User&& other) noexcept;//move assigment operatorz
	private:
		int m_id;
		std::string m_name;
		bool m_guessed = false;
		bool m_isDrawing = false;
		float score;
	};


	export std::ostream& operator<<(std::ostream& os, const User& user);

}