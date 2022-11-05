# Key Data and Functions

All information can be obtained through reverse engineering using the following tools:

- [*Cheat Engine*](https://www.cheatengine.org): An open-source memory scanner.
- [*IDA Pro*](https://hex-rays.com/ida-pro): A powerful disassembler and versatile debugger.
- [*x64dbg*](https://x64dbg.com): An open-source binary debugger for *Windows*.

## Warning

> This document is only for *Plants vs. Zombies **1.0.0.1051 CHINESE*** version.
>
> The *MD5* of `PlantsVsZombies.exe` is `37B729B4056131722A556E646AC915E9`.

## Data

### Slots

Set a breakpoint at `0x004897B2`. Each time the game is interrupted while loading a new level, the 4-bit content of `[ESP]` will be the address of the  slot structure that the game is displaying currently.

```c++
/**
 * @brief The slot.
 */
struct Slot {
    std::byte unknown1[8];

    /**
     * @brief The X-coordinate on the screen.
     * @par Offset
     * 0x8
     */
    std::int32_t screen_pos_x;

    /**
     * @brief The Y-coordinate on the screen.
     * @par Offset
     * 0xC
     */
    std::int32_t screen_pos_y;

    std::byte unknown2[36];

    /**
     * @brief The item ID.
     * @details
     * If the type of the slot is zombie, this field is equal to the zombie ID plus 0x3C.
     * If the type of the slot is plant, this field is equal to the plant ID.
     *
     * @par Offset
     * 0x34
     */
    std::int32_t id;
};
```

### The Level

- **The global base address**

  ```assembly
  mov ebx, 006A9F38h
  ```

  - **The base address of the level environment**

    ```assembly
    mov ebx, dword ptr [006A9F38h]
    mov ebx, dword ptr [ebx + 768h]
    ```

    - **The amount of sun**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 5560h]
      ```

    - **The number of zombies**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 0A0h]
      ```

    - **The number of planted plants**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 0B0h]
      ```

    - **The base address of all planted plant structures**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov ebx, dword ptr [ebx + 0ACh]
      ```

      The type of this structure is `PlantedPlant`.

    - **The countdown**

      ```assembly
      mov ebx, dword ptr [006A9F38h]
      mov ebx, dword ptr [ebx + 768h]
      mov eax, dword ptr [ebx + 5600h]
      ```

### Plants

- **Static plant information**

  ```c++
  /**
   * @brief Static plant information.
   * @par Binary Address
   * 0x0069F2B0
   */
  struct Plant {
      /**
       * @brief The ID.
       * @par Offset
       * 0x0
       */
      std::int32_t id;

      std::byte unknown1[4];

      /**
       * @brief The image resource.
       * @par Offset
       * 0x8
       */
      std::int32_t ui;

      std::byte unknown2[4];

      /**
       * @brief The cost to plant.
       * @par Offset
       * 0x10
       */
      std::int32_t sun_cost;

      /**
       * @brief The recharge time.
       * @par Offset
       * 0x14
       */
      std::int32_t recharge_time;

      std::byte unknown3[8];

      /**
       * @brief The name.
       * @par Offset
       * 0x20
       */
      char* name;
  };
  ```

  The base address of all structures is `0x0069F2B0`.

- **Planted plants in a level**

  ```c++
  /**
   * @brief The planted plant in a level.
   */
  struct PlantedPlant
  {
      std::byte unknown1[321];

      /**
       * @brief If the plant is invalid.
       * @details If this field is @p true, the plant will be removed.
       *
       * @par Offset
       * 0x141
       */
      bool invalid;

      std::byte unknown2[10];
  };
  ```

## Functions

### Game Running

- **Start a level.**

  ```c++
  /**
   * @brief Start a level.
   *
   * @param level_id  A level ID.
   * @param reset     It's @p true when starting a new level, @p false when loading the last progress.
   *
   * @par Binary Address
   * 0x0044F560
   */
  void StartLevel(
      std::int32_t level_id,
      bool reset);
  ```

- **Terminate the level.**

  ```c++
  /**
   * @brief Terminate the level.
   *
   * @param level_env The level environment.
   * @param unknown   It's always 0.
   *
   * @par Binary Address
   * 0x00413400
   */
  void EndLevel(
      void* level_env,
      std::int32_t unknown);
  ```

- **Subtract the amount of sun.**

  ```c++
  /**
   * @brief Subtract the amount of sun.
   *
   * @param count The amount to be subtracted, stored in @p EBX.
   * @return @p true if the amount of sun is enough, otherwise @p false, stored in @p AL.
   *
   * @par Binary Address
   * 0x0041BA60
   */
  bool SubtractSun(
      std::int32_t count);
  ```

- **Disable automatic pause.**

  ```assembly
  .text:0044F475  mov ecx, esi
  .text:0044F477  pop esi
  .text:0044F478  jmp sub_4502C0
  ```

  Change `jmp sub_4502C0` to `ret`.

- **Disable the running menu.**

  ```assembly
  .text:004500FC  cmp eax, esi
  .text:004500FE  mov [esp + 18h], esi
  .text:00450102  jz  short loc_450112
  ```

  Change `jz short loc_450112` to ` jmp 0045016Ah`.

- **Load the last level.**

  ```assembly
  .text:004336E6  mov esi, ecx
  .text:004336E8  jnz short loc_433766
  ```

  Change `jnz short loc_433766` to `nop`.

- **Be able to run multiple game processes simultaneously.**

  ```assembly
  .text:00553F10  call ds:GetLastError
  .text:00553F16  cmp  eax, ERROR_ALREADY_EXISTS
  .text:00553F1B  jnz  short loc_553F29
  ```

  Change `jnz` to `jmp`.

### Plants

- **Check if a plant can be planted in a certain location.**

  ```c++
  /**
   * @brief Check if a plant can be planted in a certain location.
   *
   * @param level_env The level environment.
   * @param pos_x     The X coordinate of the target location.
   * @param pos_y     The Y coordinate of the target location, stored in @p EAX.
   * @param plant_id  A plant ID.
   * @return 0 if the plant can be planted, otherwise the error code, stored in @p EAX.
   *
   * @par Binary Address
   * 0x0040E020
   */
  std::int32_t CanBePlanted(
      void* level_env,
      std::int32_t pos_x,
      std::int32_t pos_y,
      std::int32_t plant_id);
  ```

- **Create a plant.**

  ```c++
  /**
   * @brief Create a plant.
   *
   * @param level_env The level environment.
   * @param pos_x     The X coordinate of the target location.
   * @param pos_y     The Y coordinate of the target location, stored in @p EAX.
   * @param plant_id  A plant ID.
   * @param unknown   It's always -1.
   * @return Unknown.
   *
   * @par Binary Address
   * 0x0040D120
   */
  std::int32_t CreatePlant(
      void* level_env,
      std::int32_t pos_x,
      std::int32_t pos_y,
      std::int32_t plant_id,
      std::int32_t unknown);
  ```

- **Remove a plant.**

  ```c++
  /**
   * @brief Remove a plant.
   *
   * @param plant A plant.
   *
   * @par Binary Address
   * 0x004679B0
   */
  void RemovePlant(
      void* plant);
  ```

- **Remove all plants.**

  ```assembly
  .text:0041BB28  cmp [eax + 141h], bl
  .text:0041BB2E  jz  short loc_41BAE0
  ```

  Change `jz short loc_41BAE0` to `nop`.

### Zombies

- **Create a zombie.**

  ```c++
  /**
   * @brief Create a zombie.
   *
   * @param level_env The level environment, stored in @p ECX.
   * @param zombie_id A zombie ID.
   * @param pos_x     The X coordinate of the target location.
   * @param pos_y     The X coordinate of the target location, stored in @p EAX.
   * @return Unknown.
   *
   * @par Binary Address
   * 0x0042A0F0
   */
  std::int32_t CreateZombie(
      void* level_env,
      std::int32_t zombie_id,
      std::int32_t pos_x,
      std::int32_t pos_y);
  ```

- **Stop generating zombies.**

  ```assembly
  .text:0040DDD5  cmp  [esi + 0A0h], eax
  .text:0040DDDB  push edi
  .text:0040DDDC  jb   short loc_40DDE8
  ```

  Change `jb short loc_40DDE8` to `nop`.

- **Make zombies walk backwards.**

  ```assembly
  .text:0052AB21  cmp dword ptr [esi + 28h], 28h
  .text:0052AB25  jz  short loc_52AB30
  ```

  Change `jz` to `jmp`.

- **Kill all zombies.**

  ```assembly
  .text:0052AB3E  jnz loc_52ABE8
  .text:0052AB44  cmp dword ptr [esi + 6Ch], 0FFFFFFFCh
  ```

  Change these two instructions to `mov dword ptr [esi + 28h], 3`.