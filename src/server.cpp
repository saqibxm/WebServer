#include "server.hpp"

/*
 * In Object Oriented Terms
 * Think of 'sockaddr' as a Base type
 * while sockaddr_in, sockaddr_in6, sockaddr_un are derivatives.
 * functions have no concern with the derived type instead they take base type
 * which shares interface for first 1 or 2 members with derived types
 * so thats all it concerns...
 * 
 * Think of socket as a file to which we can read from and write to
 * when we write it the data written travels from port through internet connection
 * arrives at the destination port and moves to destination connection socket
 * from which it can be read (@ client side)
 * Servers start at a socket to listen the connections from, when it is established
 * a new socket (file) is alotted for that specific connection...
 */
 
 namespace fs = std::filesystem;

fs::path server_root = fs::current_path();
 
 enum class HttpVer
 {
	 HTTP_0_9,
	 HTTP_1_0,
	 HTTP_1_1,
	 HTTP_2_0
 };
 
 namespace StatusCodes
 {
	using Error = int;
	enum Info
	{
		Continue = 100,
		Processing = 102
	};
	
	enum Success
	{
		OK = 200,
		Created = 201,
		Accepted = 202
	};
	
	enum Redirect
	{
		MultipleChoices = 300,
		MovedPermanently = 301,
		Found = 302
	};
	
	enum CError
	{
		BadRequest = 400,
		Unauthorized = 401,
		Forbidden = 403,
		NotFound = 404,
		RequestTimeout = 408
	};
	
	enum SError
	{
		InternalError = 500,
		NotImplemented = 501,
		BadGateway = 502,
		Unavailable = 503,
		GatewayTimeout = 504
	};
	
	std::string to_string(Error code)
	{
		std::string str;
		switch(code)
		{
		case Info::Continue:
			str = "Continue"; break;
		case Info::Processing:
			str = "Processing"; break;
			
		case Success::OK:
			str = "OK"; break;
		case Success::Created:
			str = "Created"; break;
		case Success::Accepted:
			str = "Accepted"; break;
			
		case Redirect::MultipleChoices:
			str = "Multiple Choices"; break;
		case Redirect::MovedPermanently:
			str = "Moved Permanently"; break;
		case Redirect::Found:
			str = "Found"; break;
			
		case CError::BadRequest:
			str = "Bad Request"; break;
		case CError::Unauthorized:
			str = "Unauthorized"; break;
		case CError::Forbidden:
			str = "Forbidden"; break;
		case CError::NotFound:
			str = "Not Found"; break;
		case CError::RequestTimeout:
			str = "Request Timeout"; break;
		}
		
		return str;
	}
 }


void sleep_for_ms(unsigned ms)
{
#ifdef PLATFORM_POSIX
	usleep(ms * 1000);
#else
	Sleep(ms);
#endif
}

std::string CreateResponse(int status, std::string body)
{
	/*
	 * 1. Status line.
	 * 2. Zero or more headers, each ending with a CRLF.
	 * 3. Optional response body.
	 */

	std::string response = "HTTP/1.1";
	response.append(" " + std::to_string(status));
	response.append(" " + StatusCodes::to_string(status) + CRLF);
	response.append("Connection: Keep-Alive\n");
	response.append("Content-Type: text/html; charset=utf-8\n");
	response.append("Keep-Alive: timeout=5, max=10\n");
	if(!response.empty()) response.append(body);
	response.append(CRLF);
	return response;
}

std::string GetFileContents(const fs::path &fname)
{
	std::ifstream file(fname, std::ifstream::binary);
	if(!fs::exists(fname) || !file.is_open()) {
		std::cerr << "File can't be opened: " << std::quoted(fname.string()) << '\n';
		return "";
	}
	std::string ret(std::istreambuf_iterator<std::string::value_type>{file}, {});
	return ret;
}

std::pair<int, std::string> ProcessRequest(const std::string &request)
{	
	std::istringstream reqstrm(request);
	std::string method, path;
	
	reqstrm >> method;
	reqstrm >> path;
	
	int code = StatusCodes::NotFound;
	std::string body;
	
	if(method != "GET")
	{
		std::cerr << "Unsupported Method: " << method << '\n';
		code = StatusCodes::NotImplemented;
	}
	else
	{
        if(path == "/") { path.assign("/index.html"); }
		// path = path.substr(path.find_first_of('/') + 1); // if its npos adding 1 will reset to 0
		body = GetFileContents(server_root += path);
		code = body.empty() ? StatusCodes::NotFound : StatusCodes::OK;
	}
	
	return { code, body };
}

void Serve(int connection_fd)
{
  std::string request(1024, '\0');
  recv(connection_fd, request.data(), request.length(), 0);
  std::cout << "\n--------REQUEST BEG--------\n" << request.c_str() << "\n--------REQUEST END--------\n";
  
  auto [status, body] = ProcessRequest(request);
  std::string response = CreateResponse(status, body);
  send(connection_fd, response.c_str(), response.length(), 0);
  std::cout << "RESPONSE: " << StatusCodes::to_string(status) << '\n';
  // std::cout << "\n--------RESPONSE BEG--------\n" << response.c_str() << "--------RESPONSE END--------\n";
  
  close(connection_fd);
}

void serve(std::string root) {
  // Flush after every std::cout / std::cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  
  // You can use print statements as follows for debugging, they'll be visible when running tests.
  std::cout << "Logs from your program will appear here!\n";

  server_root = root;
  // Uncomment this block to pass the first stage
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
   std::cerr << "Failed to create server socket\n";
   return;
  }

  // Since the tester restarts your program quite often, setting SO_REUSEADDR
  // ensures that we don't run into 'Address already in use' errors
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    std::cerr << "setsockopt failed\n";
    return;
  }
  
  constexpr unsigned short PORT = 4221;
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; // take any address (address of machine or whatever)
  server_addr.sin_port = htons(PORT); // listen on 4221

  // bind server file descriptor to the specified address and attributes
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    std::cerr << "Failed to bind to port " + std::to_string(PORT) + "\n";
    return;
  }
  
  char server_ip[16];
  inet_ntop(server_addr.sin_family, &server_addr.sin_addr, server_ip, std::size(server_ip));
  std::cout << "Server Started on Port: " << PORT << std::endl;

  int connection_backlog = 5; // max amount of concurrent connections
  // listen on the server descriptor
  if (listen(server_fd, connection_backlog) != 0) {
    std::cerr << "listen failed\n";
    return;
  }

  // the structure where the address of connected client will be stored
  struct sockaddr_in client_addr; // the 'in' in sockaddr_in is not to be confused with 'input'
  int client_addr_len = sizeof(client_addr);

  std::cout << "Waiting for a client to connect...\n";

  int connection_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
  char client_ip[16] = {};
  inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip, std::size(client_ip));
  std::cout << "Client connected: " << client_ip << '\n';
  /*
  int connection_fd = 0;
  while(connection_fd = accept(server_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len)) {
	  std::cout << "Client connected\n";
	  std::thread(Serve, connection_fd).detach();
  }*/
  
  std::string request(1024, '\0');
  recv(connection_fd, request.data(), request.length(), 0);
  std::cout << "\n--------REQUEST BEG--------\n" << request.c_str() << "\n--------REQUEST END--------\n";
  
  auto [status, body] = ProcessRequest(request);
  std::string response = CreateResponse(status, body);
  send(connection_fd, response.c_str(), response.length(), 0);
  std::cout << "RESPONSE: " << StatusCodes::to_string(status) << '\n';
  // std::cout << "\n--------RESPONSE BEG--------\n" << response.c_str() << "--------RESPONSE END--------\n";
  
  close(connection_fd);
  
  close(server_fd);
}
