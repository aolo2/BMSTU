package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

import java.io.IOException;

public class JoinReducer extends Reducer<CompositeKeyWritableComparable, Text, Text, FloatWritable> {
    @Override
    protected void reduce(CompositeKeyWritableComparable key, Iterable<Text> values, Context context) throws IOException, InterruptedException {
        int numDelayedFlights = 0;
        float delaySum;
        boolean firstLine = true;
        String airportFullName = "";

        delaySum = 0.0f;

        for (Text entry : values) {
            if (firstLine) {
                airportFullName = entry.toString();
                firstLine = false;
            } else {
                delaySum += Float.parseFloat(entry.toString());;
                numDelayedFlights++;
            }
        }

        if (numDelayedFlights > 0) {
            context.write(new Text(airportFullName), new FloatWritable(delaySum / numDelayedFlights));
        }
    }
}
