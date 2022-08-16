#ifndef GAME_SCRIPT_GLOBALSCRIPTS_H
#define GAME_SCRIPT_GLOBALSCRIPTS_H

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <unordered_map>
#include <utility>
#include <variant>
#include <cstdint>

#include <components/misc/algorithm.hpp>

#include "locals.hpp"

#include "../mwworld/ptr.hpp"

namespace ESM
{
    class ESMWriter;
    class ESMReader;
    struct RefNum;
}

namespace Loading
{
    class Listener;
}

namespace MWWorld
{
    class ESMStore;
}

namespace MWScript
{
    struct GlobalScriptDesc
    {
        bool mRunning;
        Locals mLocals;
        std::variant<MWWorld::Ptr, std::pair<ESM::RefNum, std::string>> mTarget; // Used to start targeted script

        GlobalScriptDesc();

        const MWWorld::Ptr* getPtrIfPresent() const; // Returns a Ptr if one has been resolved

        MWWorld::Ptr getPtr(); // Resolves mTarget to a Ptr and caches the (potentially empty) result

        std::string_view getId() const; // Returns the target's ID -- if any
    };

    class GlobalScripts
    {
            const MWWorld::ESMStore& mStore;
            std::unordered_map<std::string, std::shared_ptr<GlobalScriptDesc>, ::Misc::StringUtils::CiHash, ::Misc::StringUtils::CiEqual> mScripts;

        public:

            GlobalScripts (const MWWorld::ESMStore& store);

            void addScript(std::string_view name, const MWWorld::Ptr& target = MWWorld::Ptr());

            void removeScript (std::string_view name);

            bool isRunning (std::string_view name) const;

            void run();
            ///< run all active global scripts

            void clear();

            void addStartup();
            ///< Add startup script

            int countSavedGameRecords() const;

            void write (ESM::ESMWriter& writer, Loading::Listener& progress) const;

            bool readRecord (ESM::ESMReader& reader, uint32_t type, const std::map<int, int>& contentFileMap);
            ///< Records for variables that do not exist are dropped silently.
            ///
            /// \return Known type?

            Locals& getLocals(std::string_view name);
            ///< If the script \a name has not been added as a global script yet, it is added
            /// automatically, but is not set to running state.

            const Locals* getLocalsIfPresent(std::string_view name) const;

            void updatePtrs(const MWWorld::Ptr& base, const MWWorld::Ptr& updated);
            ///< Update the Ptrs stored in mTarget. Should be called after the reference has been moved to a new cell.
    };
}

#endif
