# IPC Diversion Section

## Overview

* A general implementation of the Server and Client Inter-processing communication (IPC) part.
* It consists mainly of the following:
  * IPC library implementation source: src, include
  * IPC unit test program: ipc_unit_test

# Building Method

* Building by following steps:
  ```bash
  $ mkdir build
  $ cd build
  $ cmake ..
  $ make
  ```
  * The above commands can be executed with the following script:
    ```bash
    $ ./buildtest.sh
    ```

For the installation described next, which will be installed on the host PC (/usr/local/). Change the options passed to _cmake_ to change the installation destination.

```
Example 
$ cmake -DCMAKE_INSTALL_PREFIX=./install ..
```

# Installing Method

  ```bash
  $ cd build
  $ sudo make install
  ```

When succeeded, the following log will be output. 

```
[100%] Built target ipc
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/lib/pkgconfig/ipc.pc
-- Installing: /usr/local/lib/libipc.so.1.0.0
-- Installing: /usr/local/lib/libipc.so.1
-- Installing: /usr/local/lib/libipc.so
-- Installing: /usr/local/include/ipc.h
-- Installing: /usr/local/include/ipc_protocol.h
```

# Building Product 

* At last Building will generate the following:
  * \<installdir\>/include/
(External Public header files)
    ```bash
    ipc.h
    ipc_protocol.h
    ```
  * \<installdir\>/lib/
(Shared library files) 
    ```bash
    libipc.so   ( Symbolic link ) 
    libipc.so.1 ( Symbolic link )
    libipc.so.1.0.0
    ```
  * build/ipc_unit_test/
(Test program executable file)
    ```bash
    ipc_unit_test_client
    ipc_unit_test_server
    ```
<br>

# Using mehod

* This library contains functions for Server and Client, Each has different usage.

## Common of Server/Client 

* The user needs to link with the following libraries.
  * `libipc.so`
* User include the library as follow
  * #include <ipc.h>
    * 後述のipc_protocol.hはipc.h内からincludeされるため、記載不要です。ipc_protocol.h described later from ipc.h (include <ipc.h>), so description is not required.
* The header files used as follows.
  * ipc.h
    * Declare a list of available API functions.
  * ipc_protocol.h
    * Define IPC usage types and data structures for each usage.
* In libraries, Server and Client communicate using Unix Domain SocketF
  * Generate different communication files for each usage type. 
  * By default, a communication file is generated in the execution hierarchy of Server.
  * For changing the location where generating the communication files, set the environment variable "IPC_DOMAIN_PATH" 
  ```
  Example)
  $ export IPC_DOMAIN_PATH="/tmp"
    →Unix Domain Socket communication files will be generated under /tmp.
  ```

## For IC-Service

* When using this library for IC-Service, use the following values and structures (see ipc_protocol.h). 
  * UsageType：IPC_USAGE_TYPE_IC_SERVICE
  * Sending data structure：IPC_DATA_IC_SERVICE_S
  * 変化種別コールバック通知用enum Change type callback notification (enum)：IPC_KIND_IC_SERVICE_E
  * Unix Domain Socket communication File name：IpcIcService
* For IC-Service, the Cluster API library (libcluster_api.so) is the IPC Client. 
  * The API for Client is called from libcluster_api.so (described later)

##  Server API

* Server applied libipc.so can use the following APIs:
  * ipcServerStart(IPC_USAGE_TYPE_E usageType);
    * Starting the IPC Server for the specified usageType.
  * ipcSendMessage(IPC_USAGE_TYPE_E usageType, const void* pData, signed int size);
    * Sending data to the IPC Client for the specified usageType. 
    * The address and size of the sending data Specified by _pData_ and _size_ arguments. 
    * Sending data is stored in the Data Pool prepared on the IPC Client side.
  * ipcServerStop(IPC_USAGE_TYPE_E usageType);
    * Terminate the IPC Server for the specified usageType.

## Client API

* Client applied libipc.so can use the following APIs:
  * ipcClientStart(IPC_USAGE_TYPE_E usageType);
    * Starting the IPC Client for the specified usageType.
    * Connecting with IPC Server for same usageType.
  * ipcReadDataPool(IPC_USAGE_TYPE_E usageType, void* pData, signed int* pSize);
    * Reading all data in the Data Pool for the specified usageType.
    * The address where storing the read data is specified in pData, and the size of storeing data is specified in pSize.
    * The contents of the Data Pool output to pData, and the real read size output to pSize.
  * ipcRegisterCallback(IPC_USAGE_TYPE_E usageType, IPC_CHANGE_NOTIFY_CB changeNotifyCb);
    * When receiving data from the IPC Server, register the callback function for the specified usageType which receiving notification of What data changed to what.
  * ipcClientStop(IPC_USAGE_TYPE_E usageType);
    * Terminate the IPC Client for the specified usageType.

# Unit test executing method

* Limitations
  * Currently 2020/12/25, this test program connect Server and Client for IC-Service.  
  * The unix domain socket communication files are generated under /tmp/ with the file name ipcIcService.

* Since Inter processing communication takes place between ipc_unit_test_server and ipc_unit_test_client, so start each one in a separate terminal.
  Testing example as bellow (Manually operated): 

  1. **Starting Server and then starting Client**
      ```bash
      (Terminal 1)
      $ ./ipc_unit_test_server
      command (h=help, q=quit):
      ```
      ```bash
      (Terminal 2)
      $ ./ipc_unit_test_client
      command (h=help, q=quit):
      ```  
      At this point, the Server and Client connection for IC-Service completed.
      (Executing ipcServerStart () and ipcClientStart ())

  2. **Editing Server sending data, then send it**
      ```bash
      (Terminal 1)
      command (h=help, q=quit):w ←★input w 
      write command (h=help q=goto main menu):2 1 ←★input 2 1
      write command (h=help q=goto main menu):70 50 ←★input 70 50
      write command (h=help q=goto main menu):l ←★input 1
      ★Sending data list is displayed, the influnce of input contents as below. 
        2: brake(4) = 1 ←★write command 2 1 result
        70: oTempUnitVal(4) = 50 ←★write command 70 50 result
      write command (h=help q=goto main menu):q ←★input q
      command (h=help, q=quit):s ←★input s (Executing ipcSendMessage())
      ipcSendMessage return:0
      command (h=help, q=quit):
      ```
      In Client side, The callback function should be responding.
      ```bash
      (Terminal 2)
      command (h=help, q=quit):Enter changeNotifyCb ←★callback
      kind = 2, size = 4, data=1 ←★Notification of brake value changed to 1 
      Leave changeNotifyCb
      ```
      ★oTempUnitValの変化についてはIC-Serviceとしては監視対象でないのでコールバック無し。No callback for TempUnitVal change as an IC-Service, because it is not monitored .

  3. **Check Client side receiving.**  
      ```bash
      (Terminal 2)
      command (h=help, q=quit):r ←★input r
      ★Sending data list is displayed, Sending data contained as bellow. 
        2: brake(4) = 1
       70: oTempUnitVal(4) = 50
      ```

  4. **Exit Client then Server.** 
      ```bash
      (Terminal 2)
      command (h=help, q=quit):q
      bye...
      $
      ```
      ```bash
      (Terminal 1)
      command (h=help, q=quit):q
      bye...
      $
      ```

# Adding/changeing IPC usage type method
 
* First, implementetion only for IC-Service, but configured to add data for other usage easily.
* Information for each usage type is managed in the following files:
  * include/ipc_protocol.h (External Public header)
  * src/ipc_usage_info_table.c (IPC Internal Source)
* Adding information for new usage or changing information for existed useage only by two files above.
  * No changes are required for other than .c or .h files in ipc.
  * However, Regarding application and test program used IPC, It is necessary to take measures according to the adding/changing of the ipc_protocol.h definition.
* Ideally, code can be generated automatically using tools and else, but this time we don't consider that implementation.

## Sample code for adding/changing the usage type (Sample code difference)

First, the sample code for adding/changing of two files as follow.

### Example 1: When adding a new usage type

Show an difference example when adding temporarily usage type NEW_SERVICE.
Influence area of adding new usage type within the IPC.

```patch
diff --git a/include/ipc_protocol.h b/include/ipc_protocol.h
index c0ad861..2bc1115 100644
--- a/include/ipc_protocol.h
+++ b/include/ipc_protocol.h
@@ -6,6 +6,7 @@
 typedef enum {
     IPC_USAGE_TYPE_IC_SERVICE = 0,
     IPC_USAGE_TYPE_FOR_TEST,
+    IPC_USAGE_TYPE_NEW_SERVICE, // Adding usage type
     IPC_USAGE_TYPE_MAX
 } IPC_USAGE_TYPE_E;

@@ -145,4 +146,17 @@ typedef struct {
     signed int test;
 } IPC_DATA_FOR_TEST_S;

+// for IPC_USAGE_TYPE_NEW_SERVICE
+typedef enum { // Preparing only the type which we want to monitor/notify data change
+    IPC_KIND_NS_PARAM1 = 0,
+    IPC_KIND_NS_PARAM2
+} IPC_KIND_NEW_SERVICE_E;
+
+typedef struct { // This part for sending and receiving all data 
+    int param1;
+    int param2;
+    int param3;
+    int param4;
+} IPC_DATA_NEW_SERVICE_S;
+
 #endif // IPC_PROTOCOL_H
diff --git a/src/ipc_usage_info_table.c b/src/ipc_usage_info_table.c
index 976cc73..51264c6 100644
--- a/src/ipc_usage_info_table.c
+++ b/src/ipc_usage_info_table.c
@@ -51,16 +51,24 @@ static IPC_CHECK_CHANGE_INFO_S g_ipcCheckChangeForTest[] = {
     DEFINE_OFFSET_SIZE(IPC_DATA_FOR_TEST_S, test, IPC_KIND_TEST_TEST)
 };

+//   for IPC_USAGE_TYPE_FOR_TEST
+static IPC_CHECK_CHANGE_INFO_S g_ipcCheckChangeNewService[] = { // データ変化を監視・通知したい種別のみ記載 Describe only the type  you want to monitor / notify of data changes
+    DEFINE_OFFSET_SIZE(IPC_DATA_NEW_SERVICE_S, param1, IPC_KIND_NS_PARAM1),
+    DEFINE_OFFSET_SIZE(IPC_DATA_NEW_SERVICE_S, param2, IPC_KIND_NS_PARAM2)
+}; // この例では、param3, param4のデータ変化については監視・通知しない。This example donot want to monitor/notify changed of param3, param4 data
+
 // == usage info table ==
 //   index of [] is IPC_USAGE_TYPE_E
 IPC_DOMAIN_INFO_S g_ipcDomainInfoList[] =
 {
     {sizeof(IPC_DATA_IC_SERVICE_S), "ipcIcService"},
-    {sizeof(IPC_DATA_FOR_TEST_S), "ipcForTest"}
+    {sizeof(IPC_DATA_FOR_TEST_S), "ipcForTest"},
+    {sizeof(IPC_DATA_NEW_SERVICE_S), "ipcNewService"} // 新規用途用の送受信サイズ情報追加 add information of new usage sending/receiving size   
 };

 IPC_CHECK_CHANGE_INFO_TABLE_S g_ipcCheckChangeInfoTbl[] = {
     DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeIcService),
-    DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeForTest)
+    DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeForTest),
+    DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeNewService) // 新規用途用 データ変化監視テーブルを登録 Register monitor table of new usage changed data
 };

```

### 例2：既存の用途種別のデータの一部を削除する場合 Example 2: When deleting part of the existing usage type data

既存の用途IC-Serviceの送受信データから、メンバ変数brakeを削除する場合の差分例を示します。An example of the difference when the member variable brake is deleted from the transmission / reception data of the existing usage IC-Service is shown below.
IPC内における、メンバ変数を削除した場合の影響範囲となります。。Influence range of the deleting member variable within the IPC.

```patch
diff --git a/include/ipc_protocol.h b/include/ipc_protocol.h
index c0ad861..7fed8bf 100644
--- a/include/ipc_protocol.h
+++ b/include/ipc_protocol.h
@@ -13,7 +13,6 @@ typedef enum {
 typedef enum {
     IPC_KIND_ICS_TURN_R = 0,
     IPC_KIND_ICS_TURN_L,
-    IPC_KIND_ICS_BRAKE,
     IPC_KIND_ICS_SEATBELT,
     IPC_KIND_ICS_HIGHBEAM,
     IPC_KIND_ICS_DOOR,
@@ -51,7 +50,6 @@ typedef struct {
     // Telltale
     signed int turnR;
     signed int turnL;
-    signed int brake;
     signed int seatbelt;
     signed int frontRightSeatbelt;
     signed int frontCenterSeatbelt;
diff --git a/src/ipc_usage_info_table.c b/src/ipc_usage_info_table.c
index 976cc73..40ac8df 100644
--- a/src/ipc_usage_info_table.c
+++ b/src/ipc_usage_info_table.c
@@ -12,7 +12,6 @@
 static IPC_CHECK_CHANGE_INFO_S g_ipcCheckChangeIcService[] = {
     DEFINE_OFFSET_SIZE(IPC_DATA_IC_SERVICE_S, turnR, IPC_KIND_ICS_TURN_R),
     DEFINE_OFFSET_SIZE(IPC_DATA_IC_SERVICE_S, turnL, IPC_KIND_ICS_TURN_L),
-    DEFINE_OFFSET_SIZE(IPC_DATA_IC_SERVICE_S, brake, IPC_KIND_ICS_BRAKE),
     DEFINE_OFFSET_SIZE(IPC_DATA_IC_SERVICE_S, seatbelt, IPC_KIND_ICS_SEATBELT),
     DEFINE_OFFSET_SIZE(IPC_DATA_IC_SERVICE_S, highbeam, IPC_KIND_ICS_HIGHBEAM),
     DEFINE_OFFSET_SIZE(IPC_DATA_IC_SERVICE_S, door, IPC_KIND_ICS_DOOR),
```

## 用途種別の新規追加に関する共通事項 Common matters regarding the addition of a new use type

* いくつか列挙体・構造体の新規追加、および既存の列挙体・構造体内への追記を伴いますが、いずれも名称については特に制約はありません。Some new enumerations / structures will be added, and some enumerations / structures will be added to existing enumerations / structures, but there are no particular restrictions on the names of any of them.

## include/ipc_protocol.h へ追記する情報 Information added to include/ipc_protocol.h
* 1つの用途種別に対し、以下の3つの情報を追記します。
Add the following three information for one usage type.
  * 用途種別名の追記 Addition of usage type name
  * 新規用途向けの変化通知種別用列挙体の定義 Definition of enumeration for change notification type for new usage
  * 新規用途向けの送受信データ構造体の定義 Definition of send/receive data structure for new usage
* 用途種別名の追記 Addition of usage type name
  * サンプルコードの以下の部分のことになります。Sample code of this part will be as follow:
    ```patch
     typedef enum {
         IPC_USAGE_TYPE_IC_SERVICE = 0,
         IPC_USAGE_TYPE_FOR_TEST,
    +    IPC_USAGE_TYPE_NEW_SERVICE, // 用途種別追加 Adding of usage type
         IPC_USAGE_TYPE_MAX
     } IPC_USAGE_TYPE_E;
    ```
  * enum IPC_USAGE_TYPE_E 内に用途種別となるメンバを追加します。Add a member for the usage type in enum IPC_USAGE_TYPE_E.
  * IPC_USAGE_TYPE_MAXの1つ手前に追加するようにしてください(既存の定義に影響を及ぼさないようにするため)。Make sure to add it just before IPC_USAGE_TYPE_MAX (to avoid affecting existing definitions)
  * ここで定義した値は、ipc.hで定義されているipcServerStart()などの引数usageTypeへの指定用に使用します。
The value defined here is used to specify the argument usageType such as ipcServerStart() defined in ipc.h.
* 新規用途向けの変化通知種別用列挙体の定義 Definition of enumeration for change notification type for new usage
  * サンプルコードの以下の部分のことになります。Sample code of this part will be as follow:
    ```patch
    +typedef enum { // データ変化を監視したい種別のみ用意 
Only the types for which you want to monitor data changes are available
    +    IPC_KIND_NS_PARAM1 = 0,
    +    IPC_KIND_NS_PARAM2
    +} IPC_KIND_NEW_SERVICE_E;
    ```
  * データ変化通知の種別用列挙体を追加します。後述の送受信データ構造体と関連があります。Adds an enumeration for the type of data change notification. Related to the send and receive data structures described below.
  * この値は、ipcRegisterCallback()で登録されたコールバック関数の第3引数kindへの指定に使用します。This value is used to specify the callback function registered by ipcRegisterCallback () in the third argument kind.
  * 列挙体名、メンバ名について、特に名称の制約はありません。There are no specific naming restrictions for enumeration and member names
* 新規用途向けのデータ構造体の定義 Definition of send/receive data structure for new usage
  * サンプルコードの以下の部分のことになります。Sample code of this part will be as follow:
    ```patch
    +typedef struct { // この用途で送受信する全データ all data send/receive by this usage
    +    int param1;
    +    int param2;
    +    int param3;
    +    int param4;
    +} IPC_DATA_NEW_SERVICE_S;
    ```
  * 新規用途で送受信するデータ構造体を追加します。 Add data structures to send/receive for the new use.
  * IPC ServerからIPC Clientへは、ここで定義した構造体のデータ全てを送信することになります。The IPC Server will send all the data in the structure defined here to the IPC Client.

## Regarding adding src/ipc_usage_info_table.c
* 1つの用途種別に対し、以下の3つの情報を追記します。Add the following 3 information for one usage type
  * データ変化通知用の種別対応テーブルの追加 Adding a type correspondence table for data change notification
  * 通信用ドメイン情報追記(通信サイズ、ドメインファイル名) Adding communication domain information (Communication size and domain file name)
  * 用途と変化種別対応テーブルとの関係追記 Adding the relationship between usage and change type correspondence table 
* データ変化通知用の種別対応テーブルの追加 Adding a type correspondence table for data change notification
  * サンプルコードの以下の部分のことになります。Sample code of this part will be as follow:
    ```
    +//   for IPC_USAGE_TYPE_FOR_TEST
    +static IPC_CHECK_CHANGE_INFO_S g_ipcCheckChangeNewService[] = { // データ変化を監視・通知したい種別のみ記載 Describe only the type  you want to monitor / notify of data changes
    +    DEFINE_OFFSET_SIZE(IPC_DATA_NEW_SERVICE_S, param1, IPC_KIND_NS_PARAM1),
    +    DEFINE_OFFSET_SIZE(IPC_DATA_NEW_SERVICE_S, param2, IPC_KIND_NS_PARAM2)
    +}; // この例では、param3, param4のデータ変化については監視・通知しない。 In this example, data changes in param3 and param4 are not monitored or notified.
    ```
  * 新規用途向けに、IPC_CHECK_CHANGE_INFO_Sの構造体配列を追加します。For new usage, add a structure array of IPC_CHECK_CHANGE_INFO_S.
  * ipc_protocol.hで定義した変化通知種別用列挙体の定義と、通信するデータ構造体メンバを対応付けるテーブルを記載します。Describes a table that associates the definition of the change notification type enumeration defined in ipc _ protocol.h with the data structure members to be communicated.
  * このテーブルは、IPC ClientがIPC Serverからデータを受信する時に、前回受信時と変化しているデータ種別をコールバック通知する際に使用します。This table is used when the IPC Client receives data from the IPC Server and gives a callback notification of the data type changed from the previous receiving.
  * 構造体配列内には、以下のようなマクロを複数個記載して対応を定義します。 Within the structure array, describe multiple macros as shown below to define the correspondence.
    ```c
    DEFINE_OFFSET_SIZE(<データ構造体名>, <構造体メンバ名>, 変化通知列挙体メンバ名),(<Data structure name>, <Structure member name>, Change notification enumeration member name),
    ```
  * 上記サンプルコード、g_ipcCheckChangeNewService[]の場合は以下のようになります。In the case of the above sample code, g_ipcCheckChangeNewService[] will be as follows. 
    * param1が前回受信時と値が異なる場合、変化種別 IPC_KIND_NS_PARAM1 としてIPC Clientへコールバック通知する。If the value of param1 is different from the value at the time of the previous reception, the callback is notified to the IPC Client as the change type IPC_KIND_NS_PARAM1.
    * param2が前回受信時と値が異なる場合、変化種別 IPC_KIND_NS_PARAM2 としてIPC Clientへコールバック通知する。* If the value of param2 is different from the value at the time of the previous reception, the callback is notified to the IPC Client as the change type IPC_KIND_NS_PARAM2.
    * 記載していないparam3, param4については、前回受信時と値が異なっていてもコールバック通知はしない。For param3 and param4 which are not described,callback do not notify  even if the value is different from the previous receiving

* 通信用ドメイン情報追記(通信サイズ、ドメインファイル名) Adding communication domain information (Communication size and domain file name)
  * サンプルコードの以下の部分のことになります。Sample code of this part will be as follow:
    ```patch
     IPC_DOMAIN_INFO_S g_ipcDomainInfoList[] =
     {
         {sizeof(IPC_DATA_IC_SERVICE_S), "ipcIcService"},
    -    {sizeof(IPC_DATA_FOR_TEST_S), "ipcForTest"}
    +    {sizeof(IPC_DATA_FOR_TEST_S), "ipcForTest"},
    +    {sizeof(IPC_DATA_NEW_SERVICE_S), "ipcNewService"} // 新規用途用の送受信サイズ情報追加Adding of send/receive size information for new usage
     };
    ```
  * 構造体配列 g_ipcDomainInfoList[] に、新規用途向けのドメイン情報を追記します。Adding the domain information for new usage to the structure array g_ipcDomainInfoList [].
  * この追記により、新規追加した用途種別で用いる送受信データサイズと、Unix Domain Socket通信で用いるドメインファイル名が決まります。This addition determines the send / receive data size used for the newly added usage type and the domain file name used for Unix Domain Socket communication. 
  * ipc_protocol.hのenum IPC_USAGE_TYPE_Eの定義順と一致させる必要があるので、必ず末尾に追加してください。Must match the definition order of the enum IPC _ USAGE _ TYPE _ E in ipc _ protocol.h, so be sure to  added at the end.
  * 以下のように、通信するデータ構造体のサイズと、ドメインファイル名の情報を、g_ipcDomainInfoList[] の末尾に追記します。Add the size of the data structures to be communicated and the domain filename information to the end of g _ ipcDomainInfoList [], as follows:
    ```c
    {sizeof(<通信するデータ構造体名>), "ドメインファイル名"sizeof (<data structure name to communicate>), "domain file name"},
    ```
* 用途と変化種別対応テーブルとの関係追記 adding relationship matching table between usage and change type 
  * サンプルコードの以下の部分のことになります。Sample code of this part will be as follow:
    ```patch
     IPC_CHECK_CHANGE_INFO_TABLE_S g_ipcCheckChangeInfoTbl[] = {
         DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeIcService),
    -    DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeForTest)
    +    DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeForTest),
    +    DEFINE_CHANGE_INFO_TABLE(g_ipcCheckChangeNewService) // 新規用途用 データ変化監視テーブルを登録Registering a data change monitoring table for new usage
     };
    ```
  * 構造体配列 g_ipcCheckChangeInfoTbl[] に、新規用途向けの変化通知種別対応テーブルに関する情報を追記します。In the structure array g_ipcCheckChangeInfoTbl[], Adding information about the change notification type mapping table for new usage. 
  * ipc_protocol.hのenum IPC_USAGE_TYPE_Eの定義順と一致させる必要があるので、必ず末尾に追加してください。it is necessary to match the definition order of enum IPC_USAGE_TYPE_E in ipc_protocol.h,so be sure to add it at the end. 
  * 前述の「変化通知種別対応テーブル構造体」を、以下のようなマクロに記載し、g_ipcCheckChangeInfoTbl[]の末尾に追記します。Describe the above-mentioned "change notification type mapping table structure" in the following macro and add it to the end of g_ipcCheckChangeInfoTbl[].
    ```c
    DEFINE_CHANGE_INFO_TABLE(<変化通知種別対応テーブル構造体名>),DEFINE_CHANGE_INFO_TABLE (<name of change notification type mapping table structure>), 
    ```

## 既存用途向けの送信データを一部変更する場合 When partially changing the send data for existing usage
* ipc_protocol.h内の既存の送信データ構造体内のメンバ変数の削除、もしくは名称変更する場合 When deleting or renaming a member variable in an existing send data structure in ipc_protocol.h
  * ipc部分、およびipcをその用途で用いるアプリをそれぞれビルドしてみて、コンパイルエラーとなった部分を修正します。Try building the ipc part and the application that uses ipc for that purpose, and fix the part that caused the compilation error. 

* ipc_protocol.h内の既存の送信データ構想体へメンバ変数を追加する場合 When adding a member variable to an existing send data concept in ipc_protocol.h
  * [IPC用途種別の追加・変更方法](#IPC用途種別の追加・変更方法) を参考に、include/ipc_protocol.hとsrc/ipc_usage_info_table.cへの追記を行います。Refer to [How to add/change IPC usage types] (# How to add/change IPC usage types), add  to include/ipc_protocol.h and src/ipc_usage_info_table.c.

## 補足 supplement
* src/ipc_usage_info_table.cにて、DEFINE_OFFSET_SIZE()マクロにて情報を記載しているが、これはoffsetof()とsizeof()を使うことで、メンバ変数に関する構造体先頭からオフセットとサイズを取得しています。In src / ipc_usage_info_table.c, the information is described by the DEFINE_OFFSET_SIZE () macro, which uses offsetof () and sizeof () to get the offset and size of member variables from the beginning of the structure.
  * 用途種別の追加を容易に行えるようにするため、IPC処理内部ではデータ構造体内の変数名を直接指定しないような実装を行っています。In order to make it easier to add usage types, the IPC process does not directly specify variable names in data structures.
  * 各用途に対して、データ構造体のオフセットテーブルを用意することで、送信されたデータの何バイト目に何の変数があるかがわかるようになります。For each usage, by preparing an offset table of the data structure, it becomes possible to know what variables are at which byte of the send data.
    * この仕組みにより、IPC処理内部でメンバ変数名を直接指定しなくとも、データ変化の確認が可能となります。By this mechanism, it is possible to check data changes without directly specifying the member variable name inside the IPC process.
  * [IPC用途種別の追加・変更方法](#IPC用途種別の追加・変更方法) に従って用途種別を追加することで、IPC内部処理は新たな用途に対する処理ができるようになります。By adding the usage type according to [How to add / change the IPC usage type] (#How to add / change the IPC usage type), the IPC internal processing can be process the new usage.
