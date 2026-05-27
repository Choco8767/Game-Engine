#pragma once

template <typename T>
class Passkey {
    friend T;

private:
    Passkey() = default;
};
