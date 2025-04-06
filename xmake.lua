add_rules("mode.debug", "mode.release")

set_policy("package.requires_lock", true)

set_languages("c++23")

option("commonlib")
    set_default("skyrim-commonlib-ng")
option_end()

if not has_config("commonlib") then
    return
end

add_repositories("SkyrimScripting     https://github.com/SkyrimScripting/Packages.git")
add_repositories("SkyrimScriptingBeta https://github.com/SkyrimScriptingBeta/Packages.git")
add_repositories("MrowrLib            https://github.com/MrowrLib/Packages.git")

includes("xmake/*.lua")

add_requires(get_config("commonlib"))
add_requires("SkyrimScripting.Plugin", { configs = { commonlib = get_config("commonlib") } })
add_requires(
    "collections",
    "unordered_dense",
    "dependency_injection"
)

-- target("Build Papyrus Scripts")
--     set_kind("phony")
--     compile_papyrus_scripts()
    
skse_plugin({
    name = "Apprentice V2",
    version = "0.0.1",
    author = "Mrowr Purr",
    email = "mrowr.purr@gmail.com",
    src = {"plugin.cpp", "src/**.cpp"},
    packages = {
        "SkyrimScripting.Plugin",
        "collections",
        "unordered_dense",
        "dependency_injection",
    },
})

    -- mod_files = {"Scripts"},
    -- deps = {"Build Papyrus Scripts"},