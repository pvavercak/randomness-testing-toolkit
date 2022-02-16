#include "storage/istorage.h"

#include "storage/filestorage.h"

#if defined(RTT_USE_SQL)
#include "storage/mysqlstorage.h"
#endif

namespace rtt {
namespace storage {

std::unique_ptr<IStorage> IStorage::getInstance(const GlobalContainer & container) {
    switch(container.getRttCliOptions()->getResultStorageId()) {
    case Constants::ResultStorageID::FILE_REPORT:
        return FileStorage::getInstance(container);
#if defined(RTT_USE_SQL)
    case Constants::ResultStorageID::DB_MYSQL:
        return MySQLStorage::getInstance(container);
#endif
    default:
        raiseBugException("invalid result storage id");
    }


    /*if(container.getCliOptions()->getMysqlEid() != 0) {
        return MySQLStorage::getInstance(container);
    } else {
        return FileStorage::getInstance(container);
    }*/
    //return FileStorage::getInstance(container);
}

} // namespace storage
} // namespace rtt
