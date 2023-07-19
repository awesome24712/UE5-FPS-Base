#include "CoreMinimal.h"

class FPSBaseCharacter;

enum class EPermissionLevel {
	BOT = 0,		//AI player
	PUBBER = 1,		//human player, stranger
	MODERATOR = 2,	//player trusted with basic commands
	ADMIN = 3,		//player trusted with advanced commands
	OWNER = 4,		//Player is owner of server, let them do anything
	SERVER = 5,		//Server console itself
};

struct Permission {
	FString m_steamId;
	FString m_valueName; //this field is ignord, just there for server owner tracking names in the file
};

namespace PermissionSystem {

	void Init();

	//Existing permissions from the permissions.json
	void AssignOldPermissionToPlayer(FPSBaseCharacter* pPlayer);

	//Changes permissions of player to the given permission level, and saves the change to file
	void AssignNewPermissionToPlayer(EPermissionLevel perm, FPSBaseCharacter* pRequestor, FPSBaseCharacter* pAssignee);
};