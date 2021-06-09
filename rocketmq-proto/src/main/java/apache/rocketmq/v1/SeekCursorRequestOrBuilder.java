// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: apache/rocketmq/v1/service.proto

package apache.rocketmq.v1;

public interface SeekCursorRequestOrBuilder extends
    // @@protoc_insertion_point(interface_extends:apache.rocketmq.v1.SeekCursorRequest)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>.apache.rocketmq.v1.Resource group = 1;</code>
   * @return Whether the group field is set.
   */
  boolean hasGroup();
  /**
   * <code>.apache.rocketmq.v1.Resource group = 1;</code>
   * @return The group.
   */
  apache.rocketmq.v1.Resource getGroup();
  /**
   * <code>.apache.rocketmq.v1.Resource group = 1;</code>
   */
  apache.rocketmq.v1.ResourceOrBuilder getGroupOrBuilder();

  /**
   * <code>.apache.rocketmq.v1.Partition partition = 2;</code>
   * @return Whether the partition field is set.
   */
  boolean hasPartition();
  /**
   * <code>.apache.rocketmq.v1.Partition partition = 2;</code>
   * @return The partition.
   */
  apache.rocketmq.v1.Partition getPartition();
  /**
   * <code>.apache.rocketmq.v1.Partition partition = 2;</code>
   */
  apache.rocketmq.v1.PartitionOrBuilder getPartitionOrBuilder();

  /**
   * <code>.apache.rocketmq.v1.CursorPolicy policy = 3;</code>
   * @return The enum numeric value on the wire for policy.
   */
  int getPolicyValue();
  /**
   * <code>.apache.rocketmq.v1.CursorPolicy policy = 3;</code>
   * @return The policy.
   */
  apache.rocketmq.v1.CursorPolicy getPolicy();

  /**
   * <code>.google.protobuf.Timestamp time_point = 4;</code>
   * @return Whether the timePoint field is set.
   */
  boolean hasTimePoint();
  /**
   * <code>.google.protobuf.Timestamp time_point = 4;</code>
   * @return The timePoint.
   */
  com.google.protobuf.Timestamp getTimePoint();
  /**
   * <code>.google.protobuf.Timestamp time_point = 4;</code>
   */
  com.google.protobuf.TimestampOrBuilder getTimePointOrBuilder();

  /**
   * <code>int64 offset = 5;</code>
   * @return The offset.
   */
  long getOffset();
}
