#include <memory>
#include <vector>

namespace proisam
{

    class ProisamException : public std::exception {
    private:
        std::string message_;
    public:
        explicit ProisamException(const char* message) {
            message_ = message;
        }
        const char* what() const noexcept override {
            return message_.c_str();
        }
    };

    /*  PIHANDLE Flags */

    constexpr int16_t ATTACHED{ 0x01 };
    constexpr int16_t OPENED{ 0x02 };
    constexpr int16_t READONLY{ 0x04 };
    constexpr int16_t PACK{ 0x10 };
    constexpr int16_t HIT{ 0x20 };


    /*
    * Catalog flags
    */
    constexpr int16_t CAT_INUSE{ 0x01 };
    constexpr int16_t CAT_OPEN{ 0x02 };
    constexpr int16_t CAT_EXCLUSIVE{ 0x04 };
    constexpr int16_t CAT_DIRTY{ 0x08 };
    constexpr int16_t CAT_DEFER{ 0x10 };
    constexpr int16_t CAT_ACTIVE{ CAT_INUSE | CAT_OPEN };

    /*
    * PAGELOCK
    */
    enum PAGELOCKTYPE
    {
        READ_LOCK = 1,
        UPDATE_LOCK = 2,
        CORE_LOCK = 3
    };

    constexpr uint16_t IS_MAGIC{ 0xFE91 };         /* isam file magic number       */


    class ISLOCK {};

    class DISKPAGE
    {
    public:
        uint16_t nDataLen;
        uint16_t nFLink;     /* Link to next page */
        uint16_t nBLink;     /* Link to previous page */
        uint8_t nPageType;  /* Type of Page, 0 - Data, 1 - Index, 255 - Free */
        char PageData[1]; /*  Start of page data */
    };

    class DISKCAT
    {
    public:
        uint16_t nMagicNo{};      /* PRO-ISAM magic number */
        int16_t nKeyLength{};     /* File Key Length */
        int16_t nPageSize{};      /* Page Size */
        uint16_t nFree{};           /* Head of Free Page Chain */
        int16_t nHighLevel{};     /* Index High Level */
        int16_t nReservedWord{};  /* Currently Unused */
        int16_t nReservedDWord{}; /* Currently Unused */
    };

     class CLIENT
    {
    };


    class PAGELOCK
    {
    public:
        CLIENT nOwner{};         /* Owner of the update lock */
        int32_t nLockIndex{};     /* Index of the Page Lock structure */
        int16_t nFlags{};         /* Lock status */
    };

    class PAGEBUFF
    {
    public:
        int32_t nFileId{};    /* File ID */
        int32_t nFLink{};    /* Offset to next page buffer in chain */
        int32_t nBLink{};    /* Offset to previous page buffer in chain */
        PAGELOCK PageLock{};  /* Page Lock structure */
        int16_t nFlags{};     /* Page flags */
        int16_t nPageSize{};  /* Page Size */
        uint16_t nPageNo{};    /* Page Number */
        DISKPAGE DiskPage{};  /* Disk portion of the page */

        PAGEBUFF(int16_t size) : nPageSize(size) {}
        ~PAGEBUFF() {}
        //nSeekPos = (long)(nPageNo * pPageBuff->nPageSize);
        //nErr = fileRead(pCatalog->nFileRef, nSeekPos, (char*)&(pPageBuff->DiskPage), pPageBuff->nPageSize);
        std::istream& operator<<(std::istream& in);

    }; 

    class CATALOG
    {
    public:
        static bool bCatDefer;      /* TBD -- system wide */
    public:
        long nSeekPos{};   /* Current seek position */
        long nFileSize{};  /* Current file size */
        ISLOCK CatLock{};    /* Catalog Lock */
        int32_t nFileId{};    /* File Identifier */
        int32_t nRabHead{};   /* Offset to head of RAB chain */
        int32_t nDiskCatOffset{}; /* Offset in shared memory to disk catalog */
        int16_t nFlags{CAT_ACTIVE};     /* Catalog Flags */
        int16_t nOpenCount{}; /* Number of references to this catalog */
        int16_t nFileRef{};   /* Index to file handle in handle array */
        int16_t nKeyLen{};    /* Record key length */
        int16_t nPageSz{};    /* File page size */
        int16_t nFreeHead{};  /* Head of free page list */
        int16_t nHighLev{};   /* Top level index */

        std::vector<PAGEBUFF> pages{};

        CATALOG() {
            if (bCatDefer) {
                nFlags |= CAT_DEFER;
            }
        }

        std::istream& operator<<(std::istream& in);


    };


    class RAB;
    
    class IOCB
    {
    public:
        int16_t nMode{};    /* PRO-ISAM I/O mode                  */
        int16_t nBuffSz{};  /* Length of the I/O buffer           */
        uint16_t nNewPage{}; /* New leaf or branch page reference  */
        PAGELOCKTYPE nLockType{}; /* Read/Update Lock                  */
        char* pBuff{};   /* I/O buffer*/
    };

    class ProisamFile
    {
    public:
        static std::unique_ptr<ProisamFile> open(const char* filename, int16_t mode);

    public:
        std::string name{};
        int16_t nFlags{};     /* Handle flags             */
        int16_t nMode{};      /* File Access Mode         */
        CATALOG catalog{};   /* catalog entry */
        RAB* pRab{};           /* Pointer to Record Access Block entry */
        CLIENT nOwner{};       /* Handle Owner             */
        PAGEBUFF* pCurrPage{}; /* Page buffer returned by index search */
        char* pDirName{};      /* Pointer to directory name */
        char* pFileName{};     /* Pointer to file name     */
        int16_t nHandleId{};  /* Id of this handle        */
        IOCB IOcb{};           /* I/O Control Block        */

        ~ProisamFile() {}
        std::istream& operator<<(std::istream& in);

    private:
        //use the open static method.
        ProisamFile(const char* filename, int16_t mode);
    };

}
