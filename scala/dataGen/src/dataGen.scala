import java.io._
import breeze.stats.distributions.Gaussian

class cyclicMovingBody() {
  private
  def move(): Unit ={
    val dt = System.currentTimeMillis() - time
    time += dt
    val dv = acc * dt  // dv = a * t
    val dp = speed + dv * dt / 2 // dp = v + (a * t^2)/2 = v + dv * t / 2
    speed += dv // update speed
    pos += dp   // update position

    val acc_bias = 0.05 * math.sin(time / 10000 * 2 * math.Pi)  // interval is 10 seconds
    acc = Gaussian(0, 0.01).draw() + acc_bias
    acc = Math.copySign(acc, -pos)  // acc always try to bring pos to 0
  }

  override def toString() : String = acc.toString + "," + speed.toString + "," + pos.toString

  def getCurrentState(): String = {
    move()
    return toString()
  }

  private var time: Long = System.currentTimeMillis()
  private var pos: Double = 0
  private var speed: Double = 0
  private var acc: Double = 0
}

object dataGen {
  def seqGen[T](start: T, op: T => T): Stream[T] = start #:: seqGen(op(start), op)

  def main (args: Array[String]): Unit = {
    if(args.length < 2){
      println("Usage: dataGen <out_file> <approx_size>")
    }
    else {
      val outFile = args(0)
      val outMinBytes = args(1).toLong

      val seqNumber = Stream.iterate(0)(_ + 1).iterator
      val time = Stream.iterate(System.currentTimeMillis())(_ + Gaussian(500, 300).draw().asInstanceOf[Long]).iterator
      val weight = Stream.continually(Gaussian(13, 3).draw()).iterator
      val body = new cyclicMovingBody
      val bodyData = Stream.continually(body.getCurrentState()).iterator

      val iterators = List(seqNumber, time, weight, bodyData)

      val bw = new BufferedWriter(new FileWriter(outFile))
      var totalWritten: Long = 0

      val startTime = System.currentTimeMillis()
      while(totalWritten < outMinBytes)
      {
        val str = iterators.map(_.next()).mkString(",") + "\n"
        bw.write(str)
        totalWritten += str.length
      }
      bw.close()
      val totalTime_s = (System.currentTimeMillis() - startTime) / 1000
      println(s"Finished in ${totalTime_s} seconds, writing ${totalWritten / 1024/1024 / totalTime_s} Mb/s")
    }
  }
}
