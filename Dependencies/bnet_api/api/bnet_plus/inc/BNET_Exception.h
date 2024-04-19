//*****************************************************************************
// BNET_Exception.h - Exception Class for BNET
//
// Author:	Bryan Womack
// Date: 	November 10, 2017
// Company:	Echodyne Corp.
//
//*****************************************************************************

#ifndef BNET_EXCEPTION_H
#define BNET_EXCEPTION_H

#include <exception>
#include <string>

class BNET_Exception: public std::exception
{
	std::string Error_Message;

	virtual const char* what() const throw()
	{
		return const_cast<const char*>(Error_Message.c_str());
	}

	public:
		BNET_Exception(const char* error_msg)
		{
			Error_Message = std::string(error_msg) + std::string("\n");
		}

		BNET_Exception(std::string error_msg)
		{
			Error_Message = error_msg + std::string("\n");
		}
};


#endif /* BNET_EXCEPTION_H */

/* EOF */
