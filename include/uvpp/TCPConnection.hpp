#ifndef TCPConnection_hpp
#define TCPConnection_hpp

#include <functional>
#include <string>
#include "uv.h"

namespace uvpp {
	namespace internal {
		void onConnect(uv_connect_t *req, int status);
		void onRead(uv_stream_t *tcp_connection, ssize_t nread, const uv_buf_t* buf);
	}
	class TCPAcceptor;
	class TCPConnection {
		friend class TCPAcceptor;
		friend void internal::onRead(uv_stream_t *tcp_connection, ssize_t nread, const uv_buf_t* buf);
		friend void internal::onConnect(uv_connect_t *req, int status);

	public:
		using OnConnectCallback = std::function<void(void)>;
		using OnDataCallback = std::function<void(std::string)>;
		using OnErrorCallback = std::function<void(int,std::string)>;

		TCPConnection();
		TCPConnection(const TCPConnection&) = delete;
		TCPConnection(TCPConnection&&);
		TCPConnection(uv_loop_t* loop, const std::string& connect_ip, int connect_port, 
						const OnConnectCallback& on_connect = nullptr, const OnDataCallback& on_data = nullptr, const OnErrorCallback& on_error = nullptr);
		~TCPConnection();
		TCPConnection& operator=(const TCPConnection&) = delete;
    TCPConnection& operator=(TCPConnection&& other);
	
		void write(const std::string& data);

		void connect(uv_loop_t* loop, const std::string& connect_ip, int connect_port, 
						const OnConnectCallback& on_connect = nullptr, const OnDataCallback& on_data = nullptr, const OnErrorCallback& on_error = nullptr);
		void disconnect();

		bool isConnected() const;
		const std::string& ip() const;
		int port() const;

		void setOnConnectCallback(const OnConnectCallback& on_connect);
		void setOnDataCallback(const OnDataCallback& on_data);
		void setOnErrorCallback(const OnErrorCallback& on_error);
	private:
		// Private constructor to be used by TCP Acceptor in order to return a connection upon accept
		TCPConnection(uv_tcp_t* _accepted_connection, const std::string& peer_ip, int peer_port);
    void swap(TCPConnection& other);
		void uv_connect();
		void uv_read();
	private:
		bool _is_connected;

		uv_loop_t* _running_loop;
		uv_tcp_t* _tcp_connection;

		std::string _connected_to_ip;
		int _connected_to_port;

		OnConnectCallback _on_connect;
		OnDataCallback _on_data;
		OnErrorCallback _on_error;
	};
}

#include "impl/TCPConnection_impl.hpp"

#endif /* TCPConnection_hpp */