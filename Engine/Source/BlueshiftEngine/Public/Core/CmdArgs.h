#pragma once

#include "Core/WStr.h"

BE_NAMESPACE_BEGIN

class BE_API CmdArgs {
public:
    CmdArgs() { numArgs = 0; }
    CmdArgs(const wchar_t *text, bool keepAsStrings);

                        /// Copy from another.
    CmdArgs &           operator=(const CmdArgs &rhs);

                        /// Clears
    void                Clear() { numArgs = 0; }

                        /// Takes a null terminated string and breaks the string up into arg tokens.
                        /// Does not need to be /n terminated.
                        /// Set keepAsStrings to true to only seperate tokens from whitespace and comments, ignoring punctuation
    void                TokenizeString(const wchar_t *text, bool keepAsStrings);

                        /// Returns the number of arguments.
    int                 Argc() const { return numArgs; }

                        /// Returns a argument string with the given index.
                        /// Returns empty string if index is not in valid range.
    const wchar_t *     Argv(int index) const { return (index >= 0 && index < numArgs) ? argPtrs[index] : L""; }

                        /// Returns a single string containing argv(start) to argv(end) separated by ' '
    const wchar_t *     Args(int start = 0, int end = -1) const;

                        /// Tests if matched argument found.
    bool                CheckArg(const wchar_t *arg, bool caseSensitive = true) const;

                        /// Append a argument at the end of the arguments list
    void                AppendArg(const wchar_t *arg);

private:
    static const int    MaxCommandArgs = 64;
    static const int    MaxCommandString = 2048;

    int                 numArgs;                        ///< Number of arguments
    wchar_t *           argPtrs[MaxCommandArgs];        ///< Array of argument pointers that points into tokenized
    wchar_t             tokenized[MaxCommandString];    ///< text buffer
};

BE_INLINE CmdArgs::CmdArgs(const wchar_t *text, bool keepAsStrings) { 
    TokenizeString(text, keepAsStrings); 
}

BE_INLINE CmdArgs &CmdArgs::operator=(const CmdArgs &rhs) {
    numArgs = rhs.numArgs;
    memcpy(tokenized, rhs.tokenized, sizeof(tokenized));
    for (int i = 0; i < numArgs; i++) {
        int offset = rhs.argPtrs[i] - rhs.tokenized;
        argPtrs[i] = &tokenized[offset];
    }

    return *this;
}

BE_INLINE bool CmdArgs::CheckArg(const wchar_t *arg, bool caseSensitive) const {
    for (int i = 0; i < numArgs; i++) {
        if (!(caseSensitive ? WStr::Cmp(argPtrs[i], arg) : WStr::Icmp(argPtrs[i], arg)))
            return true;
    }

    return false;
}

BE_NAMESPACE_END
