//Windows VC++ での　TCP/IP サンプルプログラム（ここからはクライアント）
//入力されたデータをクライアントに送り，もらったデータを表示する
//サーバープログラムを実行してからクライアントプログラムを実行して下さい

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 5000 //サーバープログラムとポート番号を合わせてください

int main() {
	// IP アドレス，ポート番号，ソケット，sockaddr_in 構造体
	char destination[32];
	int dstSocket;
	struct sockaddr_in dstAddr;

	// 各種パラメータ
	char buffer[1024];

	// Windows の場合
	WSADATA data;
	WSAStartup(MAKEWORD(2,0), &data);

	// 相手先アドレスの入力と送る文字の入力
	printf("サーバーマシンのIPは？:");
	scanf("%s", destination);

	// sockaddr_in 構造体のセット
	memset(&dstAddr, 0, sizeof(dstAddr));
	dstAddr.sin_port = htons(PORT);
	dstAddr.sin_family = AF_INET;
	dstAddr.sin_addr.s_addr = inet_addr(destination);

	// ソケットの生成
	dstSocket = socket(AF_INET, SOCK_STREAM, 0);

	//接続
	if(connect(dstSocket, (struct sockaddr *) &dstAddr, sizeof(dstAddr))){
		printf("%s　に接続できませんでした\n",destination);
		return(-1);
	}
  	printf("%s に接続しました\n", destination);
  	printf("適当なアルファベットを入力してください\n");

	while (1){
		scanf("%s",buffer);
		//パケットの送信
		send(dstSocket, buffer, 1024, 0);
		//パケットの受信
		recv(dstSocket, buffer, 1024, 0);
		printf("→ %s\n\n",buffer);
	}

	// Windows でのソケットの終了
	closesocket(dstSocket);
	WSACleanup();
	return(0);
}
