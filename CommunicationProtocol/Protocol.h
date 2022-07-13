
typedef enum Reply
{
    SUCCESS,
    REG_FAIL_USR_EXIST,
    LOGIN_FAIL_ALREADY_ACTIVE,
    LOGIN_FAIL_WRONG_INPUT,
    GROUP_FAIL_GROUP_NOT_FOUND,
    CREATE_GROUP_FAIL_OVERFLOW,
    CREATE_GROUP_FAIL_ILLIGAL_INPUT,
    CREATE_GROUP_FAIL_DUPLICATE,
    PACK_FAIL
} Reply;

typedef enum Protocol
{
    REGISTRATION_REQUEST, /* char* Username, char* Password */
    REGISTRATION_REPLY,   /* Reply */
    LOGIN_REQUEST,        /* char* Username, char* Password */
    LOGIN_REPLY,          /* Reply */
    LOGOUT_NOTIFY,        /* char* Username */
    GROUP_JOIN_REQUEST,   /* char* Username, char* GroupName */
    GROUP_JOIN_REPLY,     /* Reply, char* GroupName, char* udpIP */
    GROUP_LEAVE,          /* char* Username, char* GroupName */
    GROUP_LIST_REQUEST,   /*  */
    GROUP_LIST_REPLY,     /* char numOfGroupsLeft, char* GroupName, int usersInGroup */
    CREATE_GROUP_REQUEST, /* char* Username, char* GroupName */
    CREATE_GROUP_REPLY    /* Reply, char* GroupName, char* udpIP */
} Protocol;

typedef struct ProtocolParams ProtocolParams;

Reply Pack(Protocol _protocol, ProtocolParams _params);

Reply UnPack(Protocol *_protocolRecieved, ProtocolParams _params);

