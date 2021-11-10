#pragma once
#include <iostream>

namespace SocketError {
	std::string GetErrorMsgText(int code);
	std::string SetErrorMsgText(std::string msgText, int code);
}
