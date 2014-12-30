#include "lyxNetException.h"

namespace lyx {

LYX_IMPLEMENT_EXCEPTION(NetException, IOException, "Net Exception")
LYX_IMPLEMENT_EXCEPTION(InvalidAddressException, NetException, "Invalid address")
LYX_IMPLEMENT_EXCEPTION(InvalidSocketException, NetException, "Invalid socket")
LYX_IMPLEMENT_EXCEPTION(ServiceNotFoundException, NetException, "Service not found")
LYX_IMPLEMENT_EXCEPTION(ConnectionAbortedException, NetException, "Software caused connection abort")
LYX_IMPLEMENT_EXCEPTION(ConnectionResetException, NetException, "Connection reset by peer")
LYX_IMPLEMENT_EXCEPTION(ConnectionRefusedException, NetException, "Connection refused")
LYX_IMPLEMENT_EXCEPTION(DNSException, NetException, "DNS error")
LYX_IMPLEMENT_EXCEPTION(HostNotFoundException, DNSException, "Host not found")
LYX_IMPLEMENT_EXCEPTION(NoAddressFoundException, DNSException, "No address found")
LYX_IMPLEMENT_EXCEPTION(InterfaceNotFoundException, NetException, "Interface not found")
LYX_IMPLEMENT_EXCEPTION(NoMessageException, NetException, "No message received")
LYX_IMPLEMENT_EXCEPTION(MessageException, NetException, "Malformed message")
LYX_IMPLEMENT_EXCEPTION(MultipartException, MessageException, "Malformed multipart message")
LYX_IMPLEMENT_EXCEPTION(HTTPException, NetException, "HTTP Exception")
LYX_IMPLEMENT_EXCEPTION(NotAuthenticatedException, HTTPException, "No authentication information found")
LYX_IMPLEMENT_EXCEPTION(UnsupportedRedirectException, HTTPException, "Unsupported HTTP redirect (protocol change)")
LYX_IMPLEMENT_EXCEPTION(FTPException, NetException, "FTP Exception")
LYX_IMPLEMENT_EXCEPTION(SMTPException, NetException, "SMTP Exception")
LYX_IMPLEMENT_EXCEPTION(POP3Exception, NetException, "POP3 Exception")
LYX_IMPLEMENT_EXCEPTION(ICMPException, NetException, "ICMP Exception")
LYX_IMPLEMENT_EXCEPTION(NTPExeption, NetException, "NTP Exception")
LYX_IMPLEMENT_EXCEPTION(HTMLFormException, NetException, "HTML Form Exception")
LYX_IMPLEMENT_EXCEPTION(WebSocketException, NetException, "WebSocket Exception")
LYX_IMPLEMENT_EXCEPTION(UnsupportedFamilyException, NetException, "Unknown or unsupported socket family.")

} // namespace lyx
