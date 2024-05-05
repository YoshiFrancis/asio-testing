
#include <cstdlib>
#include <string>
#include <array>

class chat_message
{
public:
	std::string& body()
	{
		body_ = data_.substr(header_length_);
		return body_;

	}

	const std::string& body() const
	{
		return body_;
	}

	std::string& data()
	{
		return data_;
	}

	const std::string& data() const 
	{
		return data_;
	}
	
	std::size_t length()
	{
		return header_length_ + body_length_;
	}

	void set(const std::string& message)
	{
		body_length_ = message.length();
		encode_header();
		data_ += message;
	}

	std::size_t body_length()
	{
		return body_length_;
	}

	std::size_t header_length()
	{
		return header_length_;
	}

	void body_length(std::size_t new_length)
	{
		body_length_ = new_length;
		if (body_length_ >= max_body_length_)
			body_length_ = max_body_length_;
	}

	bool decode_header()
	{
		body_length_ = 0;
		for (int idx = header_length_ - 1; idx >= 0; ++idx)
		{
			body_length_ += (body_buffer_[idx] - 48) * std::pow(10, idx);
		}
		if (body_length_ > max_body_length_)
		{
			body_length_ = 0;
			return false;
		}
		return true;
	}

	void encode_header()
	{
		std::string body_length_str = std::to_string(body_length_);
		int padding = 4 - body_length_str.size();
		for (int idx = 0; idx < padding; ++idx)
		{
			data_[idx] = '0';
		}
		for (int idx = padding; idx < body_length_str.size(); ++idx)
		{
			body_buffer_[idx] = body_length_str[idx - padding];
		}
	}

private:
	std::size_t body_length_;
	std::size_t header_length_;
	std::size_t max_body_length_ = 252;
	std::array<char, 256> body_buffer_;
	std::string data_;
	std::string body_;
};
