#pragma once
#include "IoContext.h"
#include <vector>
using namespace std;

namespace MyServer
{
	//ÿ��SOCKET��Ӧ�����ݽṹ(����GetQueuedCompletionStatus����)��
	//	-SOCKET����SOCKET��Ӧ�Ŀͻ��˵�ַ��
	//	�����ڸ�SOCKET��������(��Ӧ�ṹIoContext)��
	struct SocketContext
	{
		SOCKET m_Socket; //���ӿͻ��˵�socket
		SOCKADDR_IN m_ClientAddr; //�ͻ��˵�ַ
		//�׽��ֲ�����������WSARecv��WSASend����һ��IoContext
		vector<IoContext*> m_arrayIoContext;

		//���캯��
		SocketContext()
		{
			m_Socket = INVALID_SOCKET;
			memset(&m_ClientAddr, 0, sizeof(m_ClientAddr));
		}

		//��������
		~SocketContext()
		{
			if (m_Socket != INVALID_SOCKET)
			{
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
			}
			// �ͷŵ����е�IO����������
			for (DWORD i = 0; i < m_arrayIoContext.size(); i++)
			{
				delete m_arrayIoContext.at(i);
			}
			m_arrayIoContext.clear();
		}

		//�����׽��ֲ���ʱ�����ô˺�������PER_IO_CONTEX�ṹ
		IoContext* GetNewIoContext()
		{
			IoContext* p = new IoContext;
			m_arrayIoContext.push_back(p);
			return p;
		}

		// ���������Ƴ�һ��ָ����IoContext
		void RemoveContext(IoContext* pContext)
		{
			if (pContext == nullptr)
			{
				return;
			}
			vector<IoContext*>::iterator it;
			it = m_arrayIoContext.begin();
			while (it != m_arrayIoContext.end())
			{
				IoContext* pObj = *it;
				if (pContext == pObj)
				{
					delete pContext;
					pContext = NULL;
					it = m_arrayIoContext.erase(it);
					break;
				}
				it++;
			}
		}
	};
}