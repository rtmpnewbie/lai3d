//------------------------------------------------------------------------------
//  atomtest.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "atomtest.h"
#include "util/atom.h"

namespace Test
{
__ImplementClass(Test::AtomTest, 'atmt', Test::TestCase);

using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
AtomTest::Run()
{   
    // need to do this because other tests may have poluted the atom table
    Atom<String>::PerformGarbageCollection();
    SizeT startSize = Atom<String>::GetAtomTableSize();

    Atom<String> atom0("Bla");
    Atom<String> atom1("Blub");
    Atom<String> atom2("Bugger");

    Atom<String> atom00("Bla");
    Atom<String> atom11("Blub");
    Atom<String> atom22("Bugger");

    Atom<String> atom000(atom0);
    Atom<String> atom111(atom11);
    Atom<String> atom222(atom2);

    Atom<String> atom0000, atom1111, atom2222;

    this->Verify((Atom<String>::GetAtomTableSize() - startSize) == 3);

    this->Verify(atom0.IsValid());
    this->Verify(atom1.IsValid());
    this->Verify(atom2.IsValid());
    this->Verify(atom00.IsValid());
    this->Verify(atom11.IsValid());
    this->Verify(atom22.IsValid());
    this->Verify(atom000.IsValid());
    this->Verify(atom111.IsValid());
    this->Verify(atom222.IsValid());
    this->Verify(!atom0000.IsValid());
    this->Verify(!atom1111.IsValid());
    this->Verify(!atom2222.IsValid());

    this->Verify(atom0 != atom1);
    this->Verify(atom0 == atom00);
    this->Verify(atom0 == atom000);
    this->Verify(atom1 == atom11);
    this->Verify(atom0.Value() < atom2.Value());
    this->Verify(atom2.Value() > atom0.Value());
    
    // test that the actual contained objects are identical
    this->Verify(&atom0.Value() == &atom00.Value());
    this->Verify(&atom0.Value() == &atom000.Value());
    this->Verify(&atom00.Value() == &atom000.Value());
    this->Verify(&atom1.Value() != &atom2.Value());
    this->Verify(&atom1.Value() != &atom22.Value());
    this->Verify(&atom1.Value() == &atom111.Value());

    // test assignment from atoms and strings
    atom0000 = atom0;
    atom1111 = "Blub";
    atom2222 = atom222;
    this->Verify(atom0000 == atom0);
    this->Verify(atom0000 == atom00);
    this->Verify(atom0000 != atom1111);
    this->Verify(&atom0000.Value() == &atom0.Value());
    this->Verify(atom1111 == atom1);
    this->Verify(&atom1111.Value() == &atom1.Value());
    this->Verify(atom2222 == atom22);
    this->Verify(atom2222 == atom22);

    // test actual content
    this->Verify(atom0.Value() == "Bla");
    this->Verify(atom00.Value() == "Bla");
    this->Verify(atom000.Value() == "Bla");
    this->Verify(atom0000.Value() == "Bla");
    this->Verify(atom1.Value() == "Blub");
    this->Verify(atom11.Value() == "Blub");
    this->Verify(atom111.Value() == "Blub");
    this->Verify(atom1111.Value() == "Blub");
    this->Verify(atom2.Value() == "Bugger");
    this->Verify(atom22.Value() == "Bugger");
    this->Verify(atom222.Value() == "Bugger");
    this->Verify(atom2222.Value() == "Bugger");

    // clear content and test garbage collection
    atom0.Clear();
    atom00.Clear();
    atom000.Clear();
    atom0000.Clear();
    Atom<String>::PerformGarbageCollection();
    this->Verify((Atom<String>::GetAtomTableSize() - startSize) == 2);
    this->Verify(atom22.Value() == "Bugger");
    this->Verify(atom1.Value() == "Blub");
    atom1.Clear();
    atom11.Clear();
    atom111.Clear();
    atom1111.Clear();
    atom2.Clear();
    atom22.Clear();
    atom222.Clear();
    atom2222.Clear();
    Atom<String>::PerformGarbageCollection();
    this->Verify((Atom<String>::GetAtomTableSize() - startSize) == 0);
}

}