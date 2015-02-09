//some test data
/******************************************************************************
 *                                                                            *
 * Function: get_value                                                        *
 *                                                                            *
 * Purpose: connect to Zabbix agent, receive and print value                  *
 *                                                                            *
 * Parameters: host - server name or IP address                               *
 *             port - port number                                             *
 *             key  - item's key                                              *
 *                                                                            *
 ******************************************************************************/
static void	get_value(const char *source_ip, const char *host, unsigned short port, const char *key)//get value from agent
{
	zbx_sock_t	s;
	int		ret;
	char		request[1024];

	if (SUCCEED == (ret = zbx_tcp_connect(&s, source_ip, host, port, GET_SENDER_TIMEOUT)))
	{
		zbx_snprintf(request, sizeof(request), "%s\n", key);

		if (SUCCEED == (ret = zbx_tcp_send(&s, request)))
		{
			if (SUCCEED == (ret = SUCCEED_OR_FAIL(zbx_tcp_recv_ext(&s, ZBX_TCP_READ_UNTIL_CLOSE, 0))))
			{
				if (0 == strcmp(s.buffer, ZBX_NOTSUPPORTED) && sizeof(ZBX_NOTSUPPORTED) < s.read_bytes)
				{
					zbx_rtrim(s.buffer + sizeof(ZBX_NOTSUPPORTED), "\r\n");
					printf("%s: %s\n", s.buffer, s.buffer + sizeof(ZBX_NOTSUPPORTED));
				}
				else
				{
					zbx_rtrim(s.buffer, "\r\n");
					printf("%s\n", s.buffer);
				}
			}
		}

		zbx_tcp_close(&s);
	}

	if (FAIL == ret)
		zbx_error("Get value error: %s", zbx_tcp_strerror());
}
