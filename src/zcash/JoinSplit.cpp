// Copyright (c) 2019-2020 The Hush developers
#include "JoinSplit.hpp"
#include "prf.h"
#include "sodium.h"

#include "zcash/util.h"

#include <memory>

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <fstream>
#include "tinyformat.h"
#include "sync.h"
#include "amount.h"

#include "librustzcash.h"
#include "streams.h"
#include "version.h"

namespace libzcash {

static CCriticalSection cs_ParamsIO;

template<size_t NumInputs, size_t NumOutputs>
class JoinSplitCircuit : public JoinSplit<NumInputs, NumOutputs> {
public:
    JoinSplitCircuit() {}
    ~JoinSplitCircuit() {}
};
}
