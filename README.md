# CampusGuard: Emergency Response Coordination

COS 214 Practical 5 (2026). A C++11 application that coordinates a campus emergency from the moment it is reported until it is resolved. Security, medical, facilities and communications teams work together, access to buildings is locked or restricted, and alerts are sent over an old campus PA system that CampusGuard reaches through an adapter.

The system is one application built from six GoF design patterns working together, not six separate demos.

## Running it (Docker)

You need Docker Desktop running. From the root of this repository:

```bash
docker compose up --build
```

This builds the image (installs `g++`, `make`, `valgrind` and `gdb`, then runs `make`) and starts the program. Both scenarios run automatically, and the program ends with `CampusGuard shut down cleanly`.

### Valgrind (memory check)

```bash
docker compose --profile tools run --rm valgrind
```

Runs the program under `valgrind --leak-check=full --track-origins=yes`. Expected result: `All heap blocks were freed -- no leaks are possible` and `ERROR SUMMARY: 0 errors`.

### GDB (debugger)

```bash
docker compose --profile tools run --rm gdb
```

Opens GDB on `./campusguard` inside the container. Type `run` to start the program. The Makefile builds with `-g -O0`, so GDB shows file names, line numbers and variable values.

## Building without Docker

Requires `g++` with C++11 support and `make`:

```bash
make
./campusguard
make clean
```

The code compiles with `-std=c++11 -Wall -Wextra -pedantic -Werror`, so any warning stops the build.

## The two scenarios

**Scenario 1: Chemistry lab fire (through the Facade).** A critical fire is reported in the chemistry building (CHEM). One facade call dispatches the medical team, which makes the coordinator bring in security, facilities and comms. A PA alert goes out through the legacy system, and the incident moves to In Progress. The operator then orders an evacuation directly through the console, and finally stands down: access is restored, the incident is resolved and an all-clear is broadcast. All six patterns appear in this one flow.

**Scenario 2: Library intrusion at night (no facade).** The operator uses the console and commands directly. Security is dispatched to the library (LIB) and the library is locked. The scenario then shows four failure cases that are handled rather than ignored:

1. The PA amplifier goes offline, so the alert is rejected.
2. The operator tries to resolve the incident before work has started, which is an invalid state change.
3. The operator mistypes the area code, which is rejected as an unknown area.
4. A unit is dispatched to an incident that is already resolved, which is rejected.

It also shows cancelling a previous instruction (undo of the lock).

Every output line starts with a tag (`[Facade]`, `[Console]`, `[State]`, `[Coordinator]`, `[Adapter]`, `[LegacyPA]`, `[Dashboard]`, and so on), so each collaboration can be followed as it happens.

## Design patterns

| Pattern | Participants | What it does in CampusGuard |
|---|---|---|
| **Command** | `Command`; `DispatchUnitCommand`, `LockAreaCommand`, `IssueAlertCommand`, `EvacuateAreaCommand`; invoker `OperatorConsole` | Each operator action is an object the console runs, keeps in its history, and can undo (`cancelLast()`). |
| **Mediator** | `EmergencyMediator`; `EmergencyCoordinator`; colleagues `ResponseUnit` and `SecurityTeam`, `MedicalResponder`, `FacilitiesCrew`, `CommsTeam` | Units never talk to each other. When one is dispatched, the coordinator asks every other unit that responds to that incident to assist. |
| **Adapter** | Target `ExternalAlertService`; adapter `LegacyPAAdapter`; adaptee `LegacyCampusPASystem` | Translates CampusGuard alerts (area name, severity, any-length text) into the legacy PA's API (zone number, priority 1 to 5, text of 60 characters or less, integer error codes). |
| **Facade** | `EmergencyResponseFacade` | `respondToIncident`, `declareLockdown` and `standDown` each run three subsystem steps in order and stop at the first failure. The subsystems can still be used directly. |
| **State** (chosen) | Context `Incident`; `IncidentState`; `ReportedState`, `DispatchedState`, `InProgressState`, `ResolvedState` | Each stage decides which events are allowed. An illegal one throws `InvalidTransition`. |
| **Observer** (chosen) | Subject `Incident`; `IncidentObserver`; `OperatorDashboard`, `IncidentLogger` | Every status change is pushed to the live dashboard and the audit log. |

## Ownership

`CampusGuardSystem` owns every long-lived object. Its members are declared in a fixed order, and C++ destroys them in reverse, so anything holding a reference is destroyed before the thing it refers to. Owning relationships use `std::unique_ptr`. References to objects owned elsewhere are plain references or raw pointers. Every polymorphic base class has a virtual destructor.

## Project structure

```
include/              header files (one class or family per file)
src/                  implementation files, scenarios and main.cpp
Makefile              builds ./campusguard
Dockerfile            Ubuntu 24.04 image with g++, make, valgrind, gdb
docker-compose.yml    services: campusguard (default), valgrind and gdb (profile "tools")
```

## Team

| Member | Name | Responsibilities |
|---|---|---|
| A | Deon [surname, student number] | State and Observer: `Incident`, the incident states, dashboard and logger |
| B | Yariv [surname, student number] | Command and Adapter: commands, console, access control, legacy PA and adapter, Scenario 2, Makefile and Docker |
| C | Takunda Mugwagwa [student number] | Mediator and Facade: coordinator, response units, facade, `CampusGuardSystem`, Scenario 1, `main.cpp`, integration |
