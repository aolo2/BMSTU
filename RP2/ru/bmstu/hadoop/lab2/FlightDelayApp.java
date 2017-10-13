package ru.bmstu.hadoop.lab2;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.MultipleInputs;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;


public class FlightDelayApp {
    public static void main(String[] args) throws Exception {

        Configuration configuration = new Configuration();
        String[] options = new GenericOptionsParser(configuration, args).getRemainingArgs();

        if (options.length != 3) {
            System.err.println("Usage: FlightDelayApp <flights csv> <airports csv> <output>");
            System.exit(-1);
        }

        Job job = Job.getInstance(configuration);
        job.setJarByClass(FlightDelayApp.class);
        job.setJobName("Flight delay stats");

        MultipleInputs.addInputPath(job, new Path(options[0]), TextInputFormat.class, FlightJoinMapper.class);
        MultipleInputs.addInputPath(job, new Path(options[1]), TextInputFormat.class, AirportJoinMapper.class);
        FileOutputFormat.setOutputPath(job, new Path(options[2]));

        job.setReducerClass(JoinReducer.class);
        job.setPartitionerClass(JoinPartitioner.class);
        job.setGroupingComparatorClass(GroupingComparator.class);

        job.setMapOutputKeyClass(CompositeKeyWritableComparable.class);
        job.setMapOutputValueClass(Text.class);
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(FloatWritable.class);
        job.setNumReduceTasks(2);


        System.exit(job.waitForCompletion(true) ? 0 : 1);
    } 
} 
