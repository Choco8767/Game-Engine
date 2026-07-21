#pragma once

template <typename T>
class Passkey {
private:
    friend T;
    Passkey() = default;
};
