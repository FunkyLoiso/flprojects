mod v1 {

use std::cell::Cell;

#[derive(Clone, Default, Debug)]
struct Edge {
    val: Cell<f64>,
    diff: Cell<f64>
}

#[derive(Default, Debug)]
struct Node<'a> {
    i: Vec<Edge>,
    o: Vec<&'a Edge>
}

trait Neuron {
    fn forward(&self);
    fn backward(&self);
}

struct AddNode<'a> {
    node: Node<'a>
}

impl<'a> AddNode<'a> {
    fn new(size: usize, out: &'a Edge) -> AddNode<'a> {
        let i = vec![Default::default(); size];
        AddNode{ node: Node{ i: i, o: vec![out] } }
    }
}

impl<'a> Neuron for AddNode<'a> {
    fn forward(&self) {
        println!("forward");
        self.node.o[0].val.set( self.node.i.iter().fold(0f64, |sum, ref v| sum + v.val.get()) );   // why ref v works and &v does not?
    }

    fn backward(&self) {
        println!("backward");
        for e in &self.node.i {
            e.diff.set( self.node.o[0].diff.get() );
        }
    }
}


struct MulNode<'a> {
    node: Node<'a>
}

impl<'a> MulNode<'a> {
    fn new(size: usize, out: &'a Edge) -> MulNode<'a> {
        let i = vec![Default::default(); size];
        MulNode{ node: Node{ i: i, o: vec![out] } }
    }
}

impl<'a> Neuron for MulNode<'a> {
    fn forward(&self) {
        println!("forward");
        self.node.o[0].val.set( self.node.i.iter().fold(1f64, |mul, ref v| mul * v.val.get()) );   // why ref v works and &v does not?
    }

    fn backward(&self) {
        println!("backward");
        for i in 0..self.node.i.len() {
            self.node.i[i].diff.set( 
                self.node.o[0].diff.get() *
                self.node.i.iter()
                    .enumerate()
                    .filter( |&(i2, _)| i2 != i )
                    .fold( 1f64, |mul, (_, v)| mul * v.val.get() )
                );
        }
    }
}

}

//  a -----[ ]
//        -[+]-\
//  b ---/      -[ ]
//              -[*]---- out
//  c ---------/
fn main() {
    {
    use v1::{*};

    let out: Edge = Default::default();

    let mul = MulNode::new(2, &out);
    let add = AddNode::new(2, &mul.node.i[0]);

    let ref a = &add.node.i[0];
    let ref b = &add.node.i[1];
    let ref c = &mul.node.i[1];

    a.val.set(-2f64);
    b.val.set(5f64);
    c.val.set(-4f64);

    add.forward();
    mul.forward();

    println!("a: {:?}\nb: {:?}\nc: {:?}\no: {:?}", a, b, c, out);

    out.diff.set( if out.val.get() > 10f64 {-1f64} else {1f64} );

    mul.backward();
    add.backward();

    println!("a: {:?}\nb: {:?}\nc: {:?}\no: {:?}", a, b, c, out);
    }
}