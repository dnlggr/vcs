#include "Routing/Command.h"
#include "Routing/Router.h"

Storage *Storage::StorageInstance = 0;

int main(int argc, char *argv[]) {

    Command command(argc, argv);

    Router router(command);

    return router.execute();
}