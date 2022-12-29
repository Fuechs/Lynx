/**
 * @file fuxstr.cpp
 * @author fuechs
 * @brief fux string type
 * @version 0.1
 * @date 2022-12-29
 * 
 * @copyright Copyright (c) 2020-2022, Fuechs. All rights reserved.
 * 
 */

#include "fuxstr.hpp"

FuxStr::FuxStr(LLVMWrapper *fuxLLVM) {
    context = fuxLLVM->context;
    module = fuxLLVM->module;
    builder = fuxLLVM->builder;

    // temporary variables
    FunctionType *FT = nullptr; 
    BasicBlock *BB = nullptr;
    vector<Value *> args = {};

    // %str = type { i8*, i64, i64, i64 }
    str = StructType::create(*context, {
        builder->getInt8PtrTy(),
        builder->getInt64Ty(),
        builder->getInt64Ty(),
        builder->getInt64Ty(),
    }, "str", false);

    // %str* | ptr
    ptr = PointerType::get(str, 0);

    // define common fastcc void createDefaultStr(%str* %0)
    FT = FunctionType::get(builder->getVoidTy(), {ptr}, false);
    createDefaultStr = Function::Create(FT, Function::CommonLinkage, "createDefaultStr", *module);
    createDefaultStr->setCallingConv(CallingConv::Fast); // fastcc
    args.push_back(createDefaultStr->args().begin());
    BB = BasicBlock::Create(*context, "entry", createDefaultStr);
    builder->SetInsertPoint(BB);
    ArrayRef<Value *> idxList = { builder->getInt64(0), builder->getInt64(0) };
    builder->CreateGEP(str, args[0], idxList, "%1"); // FIXME: seg fault (11) -> why ???
    builder->CreateRetVoid();
    verifyFunction(*createDefaultStr); 
    // end of createDefaultStr

    // TODO: following functions
}