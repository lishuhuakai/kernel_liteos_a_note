__attribute__((weak)) int UseGlobalSymbol()
{
    return 0;
}

int UseGlobalSymbolImplement()
{
    return UseGlobalSymbol();
}