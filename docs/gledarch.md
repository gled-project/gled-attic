# STATUS

Utterly incomplete and neglected - last edit was around 2003.

# NAME

gledarch - Architectural Elements of `Gled`

# DESCRIPTION

This document presents the basic architectural elements of
Gled. Each of them is presented down to the implementation level (with
slight variation in the level of detail). As these elements are
often related their interdependencies are presented and
discussed. Order of presentation is from basic elements towards
complex and more entangled ones. Regarding the reading order, start
where you will ... there is a short introductory part on the
beginning of each section that should get you going.

`Gled` is still knee-deep in its development phase. Ideas for
improvements and new features are placed towards the end of each
section. Comments, intended for developers that wish to understand
`Gled` or contribute on architectural level are also included.

## Basic concepts

- Section [`Gled`: Why and how](#gled-why-and-how) presents the basic ideas of
`Gled` from the motivational standpoint. The concepts are evolved
through a series of more or less techical arguments targeted towards
justification of implementation-level details.
- Section [The `ZGlass` class](#the-zglass-class) describes functions of `ZGlass`
base-glass, its services to the `Gled` system and to a
user/programmer. Includes a discussion on data-model, glass
aggregation methods and use of inheritance when constructing glasses.
The `ZList` class is also discussed.
- [`GledNS` and `GledViewNS` namespaces](#gledns-and-gledviewns-namespaces) provide core services to
`Saturn` and `Eye`: catalogues of libsets, glasses and functions for
their handling.
- [`GledGlueGen` and `Project7`](#gledgluegen-and-project7) generate large amount of _glue_
code that enables glasses to function within the `Gled`
system. `GledGlueGen` generates code on the level of _libsets_ and
`Project7` on the level of individual glasses.
- [Saturn](#saturn) is the central entity of the `Gled` system. It is the
final authority over lens creation and lookup-by-ID as well as a
complex router and executioner of MIRs.
- [Rulers of object-spaces](#rulers-of-object-spaces) - Kings & Queens are delegated by Saturn to
rule over their entrusted object-spaces. There is a close coordination
between Saturn and rulers. Discussion on lens access and ownership is
also included here.
- [Thread support](#thread-support) enables lenses to acquire their own threads to
perform computations or dynamically modify other lenses or
lens-graphs. Within hierarchic structure of Saturns, the things get
rather complicated ...
- [Eye](#eye) is the central point of the viewing/editing/rendering
architecture of `Gled`. It serves as a router between GUI
representations of lenses and a Saturn.
- [Rendering](#rendering) infrastructure of `Gled` offers multiple variants for
traversal of lens-graphs and is fully aware of lens-aggregation
mechanisms (lists and links). It has grown rather complex to allow for
near optimal sharing of low-level geometric data and images as well as to
allow viewer-level modifications of renderer state. Core development
is focused on OpenGL, but other options are kept in mind (POV-Ray in
particular).
- [Messages & Textual Output](#messages--textual-output) analyzes possible sources of
messages in a running `Gled` cluster. Currently, only a minimal
sub-set of the desired functionality is implemented.

## Meta-concepts

- [`Gled` coding standards](#gled-coding-standards) discusses, besides code cosmetics,
usage of `C++` constructs in `Gled`: STL, exceptions and
namespaces. Further, some thoughts on threads and networking in the
`Gled` environment are presented.
- [Building and Installation](#building-and-installation)
- [Libsets](#libsets) are extensions of the `Gled` system and this section
begins with rationale behind them. Further, the available libsets and
ideas for future ones are discussed.
- [Lens-sets](#lens-sets) \*will\* be collections of lenses packed in a form of files
(or databases), accessible to a given group of users. The main
challenge is in providing efficient cross-referencing between sets.
- [Documentation & Propaganda](#documentation--propaganda): status, plans.
- [Core development plan](#core-development-plan)

## External packages

- [`ROOT`](#root): `Gled` uses `ROOT` for streaming, file access,
sockets and as its data analysis & presentation layer. This section
explains this use and also points to where `Gled` departs from
`ROOT`'s philosophy. Problems with `ROOT` and possible solutions are
discussed.
- [`FLTK`](#fltk) is used for all GUI constructs of `Gled`, from
hand-written front-ends to auto-generated widgets for glasses.

# `Gled`: Why and how

The first precursor of Gled was a simple perl/Tk application called
ZeD. It provided GUI for editing of calligraphic Bezier strokes to be
exported to Metafont or POVRay (as axially symmetric 3D tubes).  As
one can never be quite content with the first implementation of
certain tool and there was nothing really wrong with ZeD (besides
"versatile" mouse/keyboard bindings) the ideas for its reincarnation
took a most unexpected turn: ZeD was to become a multi-user
collaborative tool for editing of symbol collections. I forgot how
such a drastic change could have crossed our minds ... but there it
was and it sure seemed like the right way to go.

At that time (around 1997/98) OpenGL was slowly emerging in the Linux
world and a simple GL previewer for ZeD was written using the Fast
Light ToolKit (FLTK). Seeing things rendered in real-time is rewarding
... too rewarding as it puts imagination on fire and all kinds of
untamed ideas begin to crawl inside your head. So the plan went one
step further ... to write a multi-user tool for collaborative editing
of objects in 3D space.

There was also ROOT, the object-oriented data analysis framework
written in C++.

Having ROOT around was good, for it had object
serialization support and socket abstraction layer that worked. And it
had CINT ... which was somewhat good as it was possible to send and
execute method calls as strings and it was somewhat bad because it was
absolutely not thread safe. Anyway ... with these tools combined a
simple prototype consisting of about ten classes was scrapped up in
about half a year time. That was the first incarnation of Gled.

But there was also perl and there was the idea of parsing
C++ header files and producing tons of stuff: accessor methods,
functions for handling smart pointers and reference counting, to build
per-class widgets in fltk and later on to write all remote
method execution as auto generated creators and executors of method
invocation requests.

# The `ZGlass` class

`ZGlass` is the base-class for classes that make use of or build up
the Gled system. A Gled class is called a _glass_ an instance of
a glass is called a _lens_ (a Gled object).

## Glass identification

Each glass needs to be uniquely identifed to support lens streaming,
proper delivery of MIRs, construction and update of lens-views (GUI
and renderers) and other useful things. The identification is done by
usage of two positive integers (zero reserved for special uses):

- **Libset-id** is shared among all classes belonging to a common
library-set (build and deployment unit, see [Libsets](https://metacpan.org/pod/Libsets)). It is
assigned by hand in the libset's Makefile.
- **Class-id** identifies the glass withing the libset. It is
assigned by hand in the `glasses.list` file of each libset.

These two are joined in the FID\_t structure (short for **full ID**)
declared in `GledCore/Gled/GledTypes.h`:

    typedef UShort_t                               LID_t;
    typedef UShort_t                               CID_t;

    class FID_t {
    public:
      LID_t        lid;
      CID_t        cid;

      FID_t(LID_t l=0, CID_t c=0) : lid(l), cid(c) {}
      ...
    };

`FID_t` can be streamed as data-member and as a method argument.

To obtain a FID of a given glass one uses one of the following member
functions (both are auto-generated by `project7`):

    static  FID_t  FID();        // Use as SomeGlass::FID() or glass_ptr->FID()
    virtual FID_t VFID() const;  // Use as base_glass_ptr->VFID()

The obtained FID can then be used for further interaction with
catalogs and other Gled services (instantiators of lenses, their views
or renderers).

## Glass catalog entries

    static  GledNS::ClassInfo*  GlassInfo() { return  sap_WGlButton_ci; }
    virtual GledNS::ClassInfo* VGlassInfo() const { return  sap_WGlButton_ci; }

FID .... static + virtual ... classname

data model: basic types, root classes (stones), glasses

data access

execlock, execution ... stamps

ref counting, refcount lock

links ... copylinks .... uses execlock ... stamps

## `ZList`

List as container for links.

zlist ... copy ... listlock ... stamps

Add methods. If several links to the same lens are expected in a list,
then AddBefore/Remove methods can fail. Use only queue operations
(front & back).

need for member-type specification (to restrict membership to given glass)

`LIST_CALL #define` ... and other defines in ZGlass

## Locking

Single-glass locking can be performed at will.

Multiple-glass locks should employ all-or-nothing strategy. Not
implemented yet. Currently only needed in Saturn during execution of
MIRs. Saturn locks the Queen and the lens.

Locking done in:

a) execution of MIR

b) in auto generated Set methods

c) in ZGlass::CopyLinks(lpZglass\_t& list, Bool lockp=true) if lockp is
true.

d) in Eye::Manage during Ray processing

e) in MTW\_SubView::UpdateFromTimer\_s()

f) in RnrDriver before any direct execution of rnr methods.

## Lens creation and deletion

Queens as mothers and as grim rippers.

Creation prior to server start-up: manual creation, CheckIn(). See CINT scripts.

Creation during server operation: ZQueen::Instantiate and ZQueen::Incarnate

Deletion goes through several steps, initiated with ZQueen::RemoveLens().

1) Mark as not accepting references.

2) Unmangle self from object graph and mark as deleted. Send Viewers a
Death Ray so that they can clean-up views, widgets and renderers.

3) 

# `GledNS` and `GledViewNS` namespaces

# `GledGlueGen` and `Project7`

# Saturn

Router of messages.

MasterSocket, list of MoonSockets, list of EyeSockets.

Sun as a specific instance has no master.

All sockets handled in a single thread with common select statement.
Perhaps should separate at least the master and have it in its
own thread.

`SaturnInfo` and `SunQueen`. `EyeInfo`.

# Rulers of object-spaces

## Issues

locking rulers as atomic operation: lock all intended or unlock and wait

# Thread support

`Gled` was designed as a single-process multi-threaded
application. Threads are used within `Saturn` itself (socket
managing), one thread is dedicated to the GUI event-loop and another
one for running `ROOT`'s class `TRint` which encompasses the CINT
interpreter. These threads are not the subject of this section as they
are internal to the functioning of the `Gled` application.

User threads are spawnable from lenses of glass `Eventor`. Their
purpose and their mode of interaction with existing lenses and Saturns
can be manifold:

1. Computation performed on a single Saturn, results of which are either
broadcasted to all Saturns sharing the relevant lenses or stored on
some permanent media (or sent over the network to a data-collector).
2. Distributed computation performed on multiple Saturns, sharing some
lens-graph that serves as input to the computation. Coordination is
needed here to divide the job into suitable partitions, transmit them
to lower-level nodes and to collect their output. The final results can
be used as above.
3. Data acquisition or process spawning & monitoring. The thread connects
to certain data source, controls its operation and processes its
output. Again, the results can be used as in 1.
4. Algorithms that traverse a lens-graph and execute operations on lenses
(or graph itself), possibly performing them periodically. An example
of this class are event-engines for dynamic scenes that need to call
different lens-methods to achieve desired results. As the lens-graphs
are shared between several Saturns and changes need to be
synchronized there are two obvious options:

    - **a)** One Saturn executes the thread and emits MIRs to all Saturns sharing
    the lenses that are targets of thread's operation.
    - **b)** All Saturns execute the thread and modify local structures without
    communication with other Saturns. This can lead to dis-synchronization
    of lens-spaces and should be used with care.

    The choice depends on CPU intensity of the performed calculations,
    available bandwidth, size of resulting structures and frequency of MIR
    emission.

XXX some words on results that are re-included into gled: lens-graphs or
stones (possibly for visualization).

From the presented use-cases one can make two basic distinctions
of user-threads in `Gled`:

- Threads running on all Saturns versus threads being run on a single,
representative Saturn. This is controlled by `Multix` flag and
`Host` link.
- Threads performing intensive computation with localized interference with
external lenses versus threads performing periodic changes on
lenses with intermediate period of inactivity (sleep). For each of
these cases there is a different suspending mechanism: the first ones
must block suspending explicitly while the second ones are only
suspending during periods of inactivity.

`Gled`'s implementation of user-threads and is based on three
classes:

1. `Operator` is the glass-base for glasses that participate in
thread-traversals of lens-graphs. The `Operator` declares a virtual
method `Operate()`, which does the actual work performed by an
operator. For pure `Operator` it simply calls `Operate()` on all its
list members that are operators themselves (note `class Operator :
public ZList`). Of course, the `Operator` glass needs to be
sub-classed to export code for threading.
2. `Eventor` (sub-class of `Operator`) is the glass-base for glasses
that are a root of an operator-graph, i.e. those that actually receive
their own threads. The specialization is needed to allow for setting
of thread properties and for chores related to operation of a thread
as a whole (e.g. set-up, starting/stopping/suspending, cleaning up,
etc).
3. `Mountain` manages requests for starting, stopping, suspending and
resuming of threads for a given `Eventor`. It launches a thread
within its static member function which provides execution environment
& control together with top-level exception handling for the
`Eventor` presiding the full `Operator` hierarchy. There is a single
`Mountain` per `Gled` executable and is spawned during Saturn
initialization.

On operator level, information about thread execution type is
available and can be used to trigger different responses (e.g. send a
MIR or call a method directly). It is often impractical and time
consuming to code too many checks on every possible level and it's
perfectly OK to code operators for a given case of threading. One
should just take that into account when building operator-graphs and
providing them with execution environment.

## Thread properties & Execution environment

**Advice:** make `Operator.h`, `Eventor.h` and `Mountain.h` header
files available for inspection and peek into them as you read further.

### Properties & `Operator::Arg` argument

Thread properties define how thread(s) are spawned for a given
`Eventor`. Also, they control thread behaviour. Currently there are
five properties, declared in `Eventor.h`:

1. **`Multix`**: when `true` thread will be spawned on all Saturns,
sharing the instance of an `Eventor`. If operators change lenses,
they should do so locally, as all Saturns will mimic their behaviour.

    When `false`, the thread will only be started on a single `Saturn`
    (see next item). The lens-changes should be posted as MIRs.

    If an `Eventor` is in the fire-space of a `Saturn`, this
    distinction is meaningless.

2. **`Host`**: a _link_ to `SaturnInfo` that will execute the thread if
`Multix` is **not** set. If _null_ the thread will be started on
`Saturn` with the given `Eventor` in its sun-space.
3. The **`SignalSafe`** flag represents the second differentiation
between threads. Threads that mostly perform computation and change
lenses in short bursts are _signal-safe_ as they can be
suspended/resumed by standard signals. During the short periods when
they perform lens changes, they should lock the
`Operator::Arg::fSignalodor` mutex.

    Threads that dynamically change a scene and/or are periodically
    traversing the operator-graph are best stopped in between two
    cycles. These are _signal-unsafe_ and are internally
    suspended/resumed via the `Operator::Arg::fSuspendidor` condition
    variable. This also assures a consistent state of affected lenses at
    suspend time.

    In short, signal-safe threads tell _when not to be suspended_ while
    signal-unsafe ones offer _when they can be suspended_.

4. **`Continuous`** flag controls whether `Operate()` loop will be
entered continuously (if `true`) or it will be run just once
(`false`).
5. **`UseDynCast`** switches between usage of `dynamic_cast<Operator*`>
and direct cast to `(Operator*)` when traversing list-members of
an operator. Switch to `false` if **all** operators in the
thread-traversal are guaranteed to have only `Operator` descendants
as list-members. Relevant for large operator trees with frequent
invocation.

Upon start-up of a thread, values of these flags are copied into the
`Operator::Arg` structure, which is passed as the argument to all
calls to `Operator::Operate(Arg*)` method. This structure is
instantiated by a call to `Arg* Eventor::PreDance()`.

`Operator::Arg` is the core of user-thread execution-environment and
it encapsulates data needed for proper operation and control of a
thread inside the `Gled` system.  Additional elements of the
structure are:

- `Eventor* fEventor` which points to the root of thread's operator graph.
- `GCondition fSuspendidor` condition variable which is used to
suspend/resume signal-unsafe threads. 
- `GMutex fSignalodor` mutex which blocks sending of a suspend signal to
signal-safe threads.

The `Operator::Arg` structure can be sub-classed and extended,
therefore providing custom environment for the operators. It can also be
modified during the operate-traversal to allow for communication and
data-sharing between operators.

There is a one-thread per `Eventor` limit, but `Operator` instances
can be members of several eventor-graphs. By using data from the
`Operator::Arg` they can behave differently in each thread context.

### Thread exit & `Operator::Exception` class

Threads, that are not `Continuous` simply exit after the first
`Operate` loop. Otherwise, the loop must be exited by throwing an
exception of class `Operator::Exception`. The base exception holds a
pointer to `Operator` that has first thrown the exception, its
message (if any) and an enumeration variable of type
`Operator::Exc_e`.

There are five possible values for `Exc_e` and their meaning is
shared across the implementation of `Mountain` and `Eventor`
classes. Each of them will provoke a specific response of `Mountain`
as regards the `Eventor` in question.

- **0.** **`OE_Done`** signals normal termination of a
thread. `Eventor::PostDance(Arg*)` is called and then the thread
exits. (In fact, this is the same behaviour as for non-continuous
threads. It can be mimicked by continuous threads by setting
`Operator::Arg::fContinuous` to `false` during one `Operate` cycle.)
- **1.** **`OE_Continue`** calls `Eventor::OnContinue(Arg*, Exception&)` and,
as the name suggests, continues with whatever the thread would do
upon finishing operate-cycle without throwing an exception.
- **2.** **`OE_Wait`** calls `Eventor::OnWait(Arg*, Exception&)` and suspends
the execution of thread. It must be signaled from outside to resume
its operation.
- **3.** **`OE_Stop`** calls `Eventor::OnStop(Arg*, Exception&)` and exits the
thread.
- **4.** **`OE_Break`** calls `Eventor::OnBreak(Arg*, Exception&)` and exits the
thread.

There is a minimal difference between `OE_Stop` and `OE_Break`. In
complex `Eventors`/`Operators` it can be used to perform a different
level of clean-up for each exit condition.

## Implementation and usage details

**Note:** all relevant methods in `Operator` and `Eventor` glasses
are `virtual`.

### Operator

The code for `Operator` is trivial and its inspection should make
most things clear. Some still need to be emphasized ...

So far `Operator::Operate(Arg*)` has been discussed as a single
entity. In fact it is split into `PreOperate()`, `Operate()` and
`PostOperate()`. The `pre/post` methods must be called from the
genuine `Operate()`. This is intended for easier building of
class-hierarchies of operators.

All this methods are exception throwing. Operators can, of course,
provide lower level exception handling. This is particularly important
if you lock thread-synchronisation variables from `Arg*`.

The `Operator` glass has a void implementation of `PreOperate()` and
a trivial implementation of `Operate()` (just calling
`Pre/Post`Operate>).

The implementation of `PostOperate()` calls `Operate` on all its
list-members of type `Operator`. Its operation is further guided by
three flags:

1. **`Operator::bOpRecurse`**: if false, the descent is not done.
2. **`Arg::fUSeDynCast`** determines whether list members will be cast
dynamically (therefore allowing non-operator list-members) or
statically.
3. For each `Operator` list-member `op`, the value of its
`op-`bOpActive> flag. If it is `false`, the descent into `op` is
not done.

**`#define OP_EXE_OR_FLARE`** in the `Operator.h` file can be used by
operators to either post a MIR to `Saturn` or call a method directly, based on the
state of the `Arg::fMultix` flag. It assumes that the name of the
`Arg*` is `op_arg`. An example of its use can be found in
`Geom1:Mover` operator descendant.

### `Mountain - Eventor` coupling

`Mountain` holds a more detailed description of each thread in its
respective `DancerInfo` structure (defined in `Mountain.h`). It is
stored in a `hash_map` via an `Eventor*` key, so `Mountain` can
locate a thread belonging to a given `Eventor`.

`Mountain::Start(Eventor*)`, `Stop()`, `Suspend()` and `Resume()`
methods provide points for external access to thread status. They can
be called by anyone and are also accessible via corresponding methods
in `Eventor` (they bear the same name).

Upon change of a running-state of a thread, `Mountain` calls
appropriate methods in `Eventor` (`OnStart(Arg*)`, `OnStop(Arg*)`,
etc).  In `Eventor` itself (as a base-class) the internal thread
state flags are changed and _stamped_ to perform GUI updates. Note
that `OnStop(Arg*)` is called when thread already exits (in its
clean-up function) and is called for any exit condition that
terminates the thread.

**`Eventor`** has two pairs of methods, that wrap the actual
`Operate` loop on different levels:

- **`Pre/PostDance()`**: method `Operator::Arg* PreDance()` is called
prior to entering the true thread code. It provides the
`Operator::Arg*` that will be used throughout the thread execution. 
Then `Eventor::OnStart(Arg*)` is called.

    `PostDance(Arg*)` is called upon normal exit of the operate cycle (not
    continuous or by throwing exception with status `OE_Done`).

- **`Pre/PostBeat(Arg*)`** are enveloping the actual call to
`Eventor::Operate(Arg*)`. They are both exception throwing and can
perform checks or preparations for each operate cycle.

    If an exception is thrown from `Eventor::Operate()`, the
    `PostBeat()` method is not called.

As has been demonstrated, the `Mountain` and `Eventor` classes are
tightly coupled. But their tasks are nevertheless well
separated. `Mountain` performs all low level thread operations,
provides the main loop and calls virtual methods in `Eventor` to
perform the actual operation as well as to notify it about changes in
running state or received exceptions. `Eventor` (a glass) provides
all user-level code and exposes interface methods and thread status to
the `Gled` system.

XXX Sleeping, `InterBeatMS` (sleep kind (signal-safety), suspending
during sleep)

### `Eventor`

#### Running state of a thread

`Eventor` has three flags which reflect the running state of its
associated thread.

- `Running` and `Suspended` flags are exported and thus the hold the
same value on all Saturns. If an eventor is not `Multix` they
reflect the state of the thread in eventor's sun-space. Otherwise
these two properties hold for threads on all Saturns (for an eventor and
all its reflections).
- `Performing` flag is local (non-exported) and shows whether a thread
is running on local Saturn.

#### Eventor's event counting facilities

Event counting facilities in `Eventor` allow threads to perform a
pre-determined number of loops and then terminate. It is based on
three variables and implementation in `Eventor::PostBeat()`.

- `BeatsToDo` is the limit that should be reached by `LocBeatsDone` to
exit the thread. If `-1` runs forever (until stopped).
- `BeatsDone` is a shared variable that shows the number of beats
already done. It is updated every `StampInterval` beats and at the
exit of a thread. For `Multix` eventors this shows number of beats
done by sun-eventor, for others beats done at the performing Saturn.
- `LocBeatsDone` is **local**: it shows the number of beats done by
thread(s) on local Saturn.

The `Eventor::Reset()` method sets the last two counters to zero.

## Usage notes

### Distributed execution

time driven vs truly distributed (requests to master & result
reporting)

use of condition variables (via internal (possibly local) state)

synchronization: at finish, UDP sync, control MIRs, time driven

### On visibility of eventors

If pushing to moons is desired, eventors should be put into mandatory
queens.

sometimes desired for eventors to be invisible ... fire-space:
a) emit MIRs
b) local computation ... autostart-able comets (queens or comet-bag
with specified eventor to start; put into fire-space. could send
status updates ...

## Dependencies

### Method execution and data-access in `Gled`

Eventors and operators are critically entangled with `Gled`'s data
and method execution model. The most basic aspects are
covered by the user-thread infrastructure, the rest of them has to be
taken into account when extending `Operator` classes.

Glass data: exported versus local part

Lens context: fire, moon or sun space

Use `Saturn::PostMIR()` to submit MIRs into the system

Use lens `Set` methods to change lens data (`Set` methods do
lens-locking).

Use locks when directly accessing lens-data

### `LinuxThreads` library

All threads and thread-synchronisation methods in `Gled` use simple
class wrappers over the `LinuxThreads` library (classes `GThread`,
`GMutex` and `GCondition` in `$GLEDSYS/Gled/Gled` directory).

## Issues

naming, Mountain as glass (Queen?), inter-thread comm, inner threads
(or sub-threads), multix reversal based on Saturn depth (reasonable
for local reflectors)

`Multix` threads for newly connected Saturns are started via
`Eventor::AdEnlightenment`.

non SignalSafe threads should have option to call consider suspend
(dancer info not in Op::Arg)

`LinuxThreads` ... posix? non-portable extension in mutexes.
Perhaps Mountain should be made OS specific?

ownership

PostMIR ... locks; also ownership

Saturn services (e.g. triangulators ... in fire-space?)

`Operator::CopySubOps()`

ROOT & threads: the TFile problem (networking ok?) ... current solution

Operators vs queens; should threads register to queens they intend to
change (to suspend them prior to streaming)?

# Eye

`OptoStructs`, FTW, MTW weeds

auto-generated weeds ... their future

## Locators

Locator identifies a particular element in a `FTW_Nest`. It holds
pointers to _nest_, _leaf_ and _ant_ (leaf and ant can be
zero). They are used to represent nest's _point_, _mark_ and
_target_ and for direct setting of _shell_-level _source_ and
_sink_. They will probably be used for other purposes as well when
the GUI structure becomes more general.

Point and mark locators are handled internally by the nest. Other
locators bound to the same nest are owned by the nest as it
offers basic locator management. Classes that use own locators should
be sub-classed from `FTW::LocatorConsumer` class and
register/unregister themselves with the nest in question.

User locators must be instantiated prior to registration. Nest keeps a
list of `LocatorConsumer`s associated with each `Locator` and when
the list becomes empty the nest deletes this `Locator`.

Locators report their changes to the nest (by calling
`FTW_Nest::LocatorChange(Locator&)`). Nest forwards the notification to all
registered consumers by calling `LocatorConsumer::locator_change`.

If the leaf pointed to by some locators is removed, the locator is
_migrated_ in the same manner as point and mark are.

`LocatorConsumer` class also provides basic interface (with
register/unregister functionality implemented) for changing the used
locator (called `base`): `set_base(Locator&)` and `clear_base()`.

During nest destruction all consumers are notified with
`LocatorConsumer::destroy_base` method call.

See `FTW::Locator_Selector` for an example.

## Issues

MTW\_View: per class collapsor, refresh button, periodic refresh

# Rendering

## `ZNode`: basic glass implementation of a hierarchic model

The `ZNode` glass provides all elements needed for implementation of
a hierarchic model of 3D graphics. It contains geometrical
transformation from its parent, a link to parent and is subclasses
from `ZList` which provides storage for holding children.

### Tree structure

`ZNode` has a link `ZNode* mParent` which can point to its parent
node. As a `ZList` descendant, it can contain links to its
children. Since `ZList` membership can not be restricted by type
(yet; anyway it could be restricted in `ZNode::Add*` methods) it is
not guaranteed that all its members will actually be `ZNode`s.

`ZNode`'s `Add` methods are overloaded to set parent of the new
member (if it is a `ZNode` and its parent is `0`) to `this`.

Problem arises, when a node is added to a second parent. Say, `N` is a
child of `A`. Then `N` is added as child to another node
`B`. Should `N`'s parent be set to `B` or remain set to `A`?

There is no general solution although in most cases the answer is
no. To control the behaviour of `ZNode::Add*` methods there is a
`Bool_t bKeepParent` flag declared in `ZNode`. If it is **true** then
`N`'s parent remains `A` and `N` remains one of `A`'s children.
If it is **false** then `N`'s parent is set to `B` and `N` is
removed from the list of `A`'s children.

### Representation of geometric transformation

The transformation is stored in a stone `ZTrans`, which in turn is a
wrapper around `5x5` `ZMatrix`. It is a mixture of the Lorenz
transformation and homogeneous coordinates. The 0th row and column of
the matrix are time components. Its sub matrix formed by 1st to 3rd
rows is the standard rotation matrix. The 4th column represents the
position four vector + scaling factor.

!!! Check what is taken for building GL matrix.

Scale and shearing variables are not used. The time variables are
likewise unused. Lorentz transformations are not implemented. Time is
seen just as another dimension and can be rotated to/from by passing
axis argument of 0 to `ZNode::RotateLF(int axis1, int axis2, float
amount)`. 

Admittedly, this is rather peculiar. 

# Messages & Textual Output

Saturn level

Eye level

Take `const string&` or `const Text_t*`

Provide formatting tools: `GForm(const char* fmt, ...)` is available
and thread safe.

Redirection, duplication of streams

# `Gled` coding standards

`Gled` follows most of the rules used for ROOT (see:
 http://root.cern.ch/root/Conventions.html ).

For important classes follow one class per file rule. This makes life
with auto-generated code much easier.

Class names are capitalized, the most "raw" classes of `Gled` begin
with Z. If you need Get/Set methods, add export comment to the
declaration:

    Int_t         mWidth;   // X{gs}

and include `#include "file-stem.h7"` somewhere towards the end of
class definition. See section on `project7` parser.

Private and protected data members begin with `m` as in `mFoo`. Public
members begin with `f` as in `fField`.

Enumeration types end with `_e` and individual values should begin
with some common prefix:

    class Top_Selector {
     public:
       enum Type_e { T_Undef=-1, T_Locator, T_Inst, T_DevNull, T_DND };
    ...
    };

STL constructs can be typedef-ed (I do that most of the time):

    typedef list<ZGlass*>                   lpZGlass_t;
    typedef list<ZGlass*>::iterator         lpZGlass_i;
    typedef list<ZGlass*>::const_iterator   lpZGlass_ci;
    typedef list<ZGlass*>::reverse_iterator lpZGlass_ri;

    typedef list<ZGlass**>                  lppZGlass_t;
    typedef list<ZGlass**>::iterator        lppZGlass_i;

    typedef map<ID_t, ZGlass*>                      mID2pZGlass_t;
    typedef map<ID_t, ZGlass*>::iterator            mID2pZGlass_i;
    typedef hash_map<ID_t, ZGlass*>                 hID2pZGlass_t;
    typedef hash_map<ID_t, ZGlass*>::iterator       hID2pZGlass_i;

In particular: prefix with container type and `p`'s for
pointers. Postfix with `t` for type, `i` for iterator, etc ...

Use templates at your own discretion. I rarely use them for other
stuff than containers.

Namespaces are good. Use them. Especially useful for importing foreign
software (see `GTS` libset as an example).

## About ROOT

ROOT classes that are streamable must have a default constructor. This
pertains to all glasses and stones.

Always initialize the relevant data in constructor. I use private
method `void _init()` for that.

## Issues

Perhaps libsets should have their own namespaces.

# Building and Installation

directory layout

configure

standard Makefiles in $GLEDSYS/make

make\_\*.inc includes generated by configure 

standard targets

double-collon rules: 

# Libsets

Libsets are a means of extending `Gled`. The "set" part of the name
tries to imply that each libset is in fact composed of several
libraries (e.g. `libGled.so`, `libGled_View.so` and
`libGled_Rnr_GL.so`):

1. Core or base library. It contains glasses, stones and anything
necessary for functioning of the system.
2. View library contains all GUI components: handcrafted as well as auto
generated.
3. Rnr libraries, one for each rendering engine (so far only GL rendering
is supported).

Each libset should in principle implement a larger chunk of
functionality. For example, the `Gled` libset provides basic
infrastructure classes (glasses and stones), implementation of
`Saturn`, basic GUI elements and rendering support as well as a
complete application framework realized in two executables (`gled`
and `saturn`).

`Geom1` libset implements some geometrical shapes together with
(unoptimized) GL renderers. There are also a few `Operator`s useful for
simple animations.

`Numerica` will hold implementations of numerical methods interesting
for wider public. So far only an ODE integrator (`ODECrawler`) and
amoeba crossed with Metropolis algorithm (`WarmAmoeba`) are available.

## Anatomy of a libset

## Creating a libset

## Extending libset with new classes

# Lens-sets

XXX description on meta level.

No lens-sets are available. Unsure of the best implementation and
protocol.

Help badly needed.

# Documentation & Propaganda

There is \*some\* documentation available. There is a strong desire to
have more of that stuff.

Audience to guide the documentation project would be helpful. What
parts will be written first and partially also when and at what rate
depends on user/developer interest.

Propaganda will be dispatched to mailing lists of familiar projects
with main goal of attracting developers and supporters (fans if you
wish).

Wide announcements will probably happen toward the end of 2003.

# Core development plan

Stabilize code. Finalize paradigms. Some aspects of `Gled` are still
floating.

Prepare stones/GUI for physics analysis. Mainly creation and viewing
of event objects and ROOT histograms and trees.

Focus on sturdy base with usable GUI and rendering.

Provide some cluster/multi-user applications.

Prepare simple mechanisms for file access based on URI. Put this
in context of glasses and lens-graphs.

Implement (or envelop) elements for user authentication, group and
virtual organization management.

Begin planning for version 2. Implement in `v1` things that are
possible.

# `ROOT`

See http://root.cern.ch/.

`ROOT` is great software. Without it, writing `Gled` would be a
never-ending nightmare.

# `FLTK`

See http://www.fltk.org/.
